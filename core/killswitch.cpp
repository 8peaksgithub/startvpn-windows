#include "killswitch.h"
#include "common/logger.h"
#include <QProcess>
#include <QDebug>

namespace Start {

const QString KillSwitch::RULE_NAME_BLOCK_OUT = "StartVPN_KillSwitch_BlockOut";
const QString KillSwitch::RULE_NAME_BLOCK_IN = "StartVPN_KillSwitch_BlockIn";
const QString KillSwitch::RULE_NAME_VPN_OUT = "StartVPN_KillSwitch_AllowVPNOut";
const QString KillSwitch::RULE_NAME_VPN_IN = "StartVPN_KillSwitch_AllowVPNIn";

KillSwitch::KillSwitch(QObject *parent)
    : QObject(parent)
    , m_enabled(false)
{
}

KillSwitch::~KillSwitch()
{
    // Ensure kill switch is disabled on destruction to restore connectivity
    if (m_enabled) {
        disable();
    }
}

bool KillSwitch::enable()
{
    if (m_enabled) {
        Logger::instance().addMessage("Kill switch already enabled");
        return true;
    }

    Logger::instance().addMessage("Enabling kill switch...");

    // First, remove any existing rules to start fresh
    removeFirewallRules();

    // Create blocking rules
    if (!createBlockRules()) {
        m_lastError = "Failed to create firewall blocking rules";
        Logger::instance().addMessage(m_lastError);
        return false;
    }

    m_enabled = true;
    Logger::instance().addMessage("Kill switch enabled successfully");
    return true;
}

bool KillSwitch::disable()
{
    if (!m_enabled) {
        Logger::instance().addMessage("Kill switch already disabled");
        return true;
    }

    Logger::instance().addMessage("Disabling kill switch...");

    if (!removeFirewallRules()) {
        m_lastError = "Failed to remove firewall rules";
        Logger::instance().addMessage(m_lastError);
        return false;
    }

    m_enabled = false;
    m_vpnInterfaceName.clear();
    Logger::instance().addMessage("Kill switch disabled successfully");
    return true;
}

bool KillSwitch::isEnabled() const
{
    return m_enabled;
}

bool KillSwitch::allowVPNInterface(const QString& interfaceName)
{
    if (interfaceName.isEmpty()) {
        m_lastError = "Invalid interface name";
        return false;
    }

    m_vpnInterfaceName = interfaceName;
    Logger::instance().addMessage(QString("Allowing traffic through VPN interface: %1").arg(interfaceName));

    // Remove old VPN allow rules if they exist
    executeNetshCommand(QString("advfirewall firewall delete rule name=\"%1\"").arg(RULE_NAME_VPN_OUT));
    executeNetshCommand(QString("advfirewall firewall delete rule name=\"%1\"").arg(RULE_NAME_VPN_IN));

    // Allow outbound traffic through VPN interface
    QString cmdOut = QString("advfirewall firewall add rule name=\"%1\" dir=out action=allow "
                            "interfacetype=any enable=yes profile=any")
                            .arg(RULE_NAME_VPN_OUT);
    
    if (!executeNetshCommand(cmdOut)) {
        m_lastError = "Failed to create VPN outbound allow rule";
        return false;
    }

    // Allow inbound traffic through VPN interface
    QString cmdIn = QString("advfirewall firewall add rule name=\"%1\" dir=in action=allow "
                           "interfacetype=any enable=yes profile=any")
                           .arg(RULE_NAME_VPN_IN);
    
    if (!executeNetshCommand(cmdIn)) {
        m_lastError = "Failed to create VPN inbound allow rule";
        return false;
    }

    Logger::instance().addMessage("VPN interface traffic allowed");
    return true;
}

QString KillSwitch::lastError() const
{
    return m_lastError;
}

bool KillSwitch::executeNetshCommand(const QString& command)
{
    QString fullCommand = "netsh " + command;
    Logger::instance().addMessage(QString("Executing: %1").arg(fullCommand));

    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << fullCommand);
    
    if (!process.waitForStarted(5000)) {
        m_lastError = "Failed to start netsh command";
        return false;
    }

    if (!process.waitForFinished(10000)) {
        m_lastError = "Netsh command timed out";
        process.kill();
        return false;
    }

    int exitCode = process.exitCode();
    QByteArray output = process.readAllStandardOutput();
    QByteArray errorOutput = process.readAllStandardError();

    if (exitCode != 0) {
        m_lastError = QString("Netsh command failed with exit code %1: %2")
                          .arg(exitCode)
                          .arg(QString::fromLocal8Bit(errorOutput));
        Logger::instance().addMessage(m_lastError);
        return false;
    }

    if (!output.isEmpty()) {
        Logger::instance().addMessage(QString("Command output: %1").arg(QString::fromLocal8Bit(output)));
    }

    return true;
}

bool KillSwitch::removeFirewallRules()
{
    bool success = true;

    // Remove block rules
    if (ruleExists(RULE_NAME_BLOCK_OUT)) {
        if (!executeNetshCommand(QString("advfirewall firewall delete rule name=\"%1\"").arg(RULE_NAME_BLOCK_OUT))) {
            success = false;
        }
    }

    if (ruleExists(RULE_NAME_BLOCK_IN)) {
        if (!executeNetshCommand(QString("advfirewall firewall delete rule name=\"%1\"").arg(RULE_NAME_BLOCK_IN))) {
            success = false;
        }
    }

    // Remove VPN allow rules
    if (ruleExists(RULE_NAME_VPN_OUT)) {
        if (!executeNetshCommand(QString("advfirewall firewall delete rule name=\"%1\"").arg(RULE_NAME_VPN_OUT))) {
            success = false;
        }
    }

    if (ruleExists(RULE_NAME_VPN_IN)) {
        if (!executeNetshCommand(QString("advfirewall firewall delete rule name=\"%1\"").arg(RULE_NAME_VPN_IN))) {
            success = false;
        }
    }

    return success;
}

bool KillSwitch::createBlockRules()
{
    // Create outbound blocking rule with lower priority (will be overridden by VPN allow rules)
    QString cmdOut = QString("advfirewall firewall add rule name=\"%1\" dir=out action=block "
                            "enable=yes profile=any")
                            .arg(RULE_NAME_BLOCK_OUT);
    
    if (!executeNetshCommand(cmdOut)) {
        m_lastError = "Failed to create outbound blocking rule";
        return false;
    }

    // Create inbound blocking rule
    QString cmdIn = QString("advfirewall firewall add rule name=\"%1\" dir=in action=block "
                           "enable=yes profile=any")
                           .arg(RULE_NAME_BLOCK_IN);
    
    if (!executeNetshCommand(cmdIn)) {
        m_lastError = "Failed to create inbound blocking rule";
        // Try to cleanup
        executeNetshCommand(QString("advfirewall firewall delete rule name=\"%1\"").arg(RULE_NAME_BLOCK_OUT));
        return false;
    }

    return true;
}

bool KillSwitch::ruleExists(const QString& ruleName)
{
    QProcess process;
    QString command = QString("netsh advfirewall firewall show rule name=\"%1\"").arg(ruleName);
    
    process.start("cmd.exe", QStringList() << "/c" << command);
    
    if (!process.waitForStarted(5000)) {
        return false;
    }

    if (!process.waitForFinished(10000)) {
        process.kill();
        return false;
    }

    QByteArray output = process.readAllStandardOutput();
    QString outputStr = QString::fromLocal8Bit(output);

    // If rule doesn't exist, output will contain "No rules match"
    return !outputStr.contains("No rules match", Qt::CaseInsensitive);
}

} // namespace Start
