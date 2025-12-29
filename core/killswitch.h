#ifndef KILLSWITCH_H
#define KILLSWITCH_H

#include <QObject>
#include <QString>
#include <QProcess>

namespace Start {

/**
 * @brief The KillSwitch class implements a VPN kill switch to prevent IP leaks
 * 
 * This class manages Windows Firewall rules to block all traffic when VPN is enabled,
 * except for traffic through the VPN interface. This prevents IP address leaks if the
 * VPN disconnects unexpectedly.
 */
class KillSwitch : public QObject
{
    Q_OBJECT

public:
    explicit KillSwitch(QObject *parent = nullptr);
    ~KillSwitch();

    /**
     * @brief Enable the kill switch
     * @return true if successful, false otherwise
     */
    bool enable();

    /**
     * @brief Disable the kill switch and restore normal connectivity
     * @return true if successful, false otherwise
     */
    bool disable();

    /**
     * @brief Check if kill switch is currently enabled
     * @return true if enabled, false otherwise
     */
    bool isEnabled() const;

    /**
     * @brief Allow traffic through specific VPN interface
     * @param interfaceName Name of the VPN network interface
     * @return true if successful, false otherwise
     */
    bool allowVPNInterface(const QString& interfaceName);

    /**
     * @brief Get the last error message
     * @return Error message string
     */
    QString lastError() const;

private:
    bool m_enabled;
    QString m_lastError;
    QString m_vpnInterfaceName;
    static const QString RULE_NAME_BLOCK_OUT;
    static const QString RULE_NAME_BLOCK_IN;
    static const QString RULE_NAME_VPN_OUT;
    static const QString RULE_NAME_VPN_IN;

    /**
     * @brief Execute netsh command to manage firewall rules
     * @param command Command string to execute
     * @return true if successful, false otherwise
     */
    bool executeNetshCommand(const QString& command);

    /**
     * @brief Remove all kill switch firewall rules
     * @return true if successful, false otherwise
     */
    bool removeFirewallRules();

    /**
     * @brief Create firewall rules to block all traffic
     * @return true if successful, false otherwise
     */
    bool createBlockRules();

    /**
     * @brief Check if a specific firewall rule exists
     * @param ruleName Name of the rule to check
     * @return true if rule exists, false otherwise
     */
    bool ruleExists(const QString& ruleName);
};

} // namespace Start

#endif // KILLSWITCH_H
