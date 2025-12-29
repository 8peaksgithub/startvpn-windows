# StartVPN Windows - Kill Switch Quick Reference

## What is the Kill Switch?

The kill switch is a security feature that automatically blocks all internet traffic when your VPN connection drops, preventing your real IP address from being exposed.

## How It Works

### Automatic Operation
The kill switch operates automatically during the VPN lifecycle:

1. **On Connect**: Kill switch activates, blocking all non-VPN traffic
2. **Connected**: VPN traffic is allowed through the firewall
3. **On Disconnect**: Kill switch deactivates, restoring normal internet access
4. **On Unexpected Drop**: Traffic remains blocked until VPN reconnects

### Technical Details

#### Windows Firewall Rules
The application creates the following firewall rules:
- `StartVPN_KillSwitch_BlockOut` - Blocks outbound traffic
- `StartVPN_KillSwitch_BlockIn` - Blocks inbound traffic  
- `StartVPN_KillSwitch_AllowVPNOut` - Allows VPN outbound traffic
- `StartVPN_KillSwitch_AllowVPNIn` - Allows VPN inbound traffic

#### Requirements
- Windows Administrator privileges (required for firewall management)
- Windows Firewall must be enabled
- Compatible with Windows 7, 8, 10, and 11

## Manual Control (Advanced)

While the kill switch operates automatically, developers can control it programmatically:

```cpp
// Enable kill switch manually
Start::Common::globalConnection()->enableKillSwitch();

// Disable kill switch manually
Start::Common::globalConnection()->disableKillSwitch();

// Check if enabled
bool enabled = Start::Common::globalConnection()->killSwitch()->isEnabled();

// Get error message if operation fails
QString error = Start::Common::globalConnection()->killSwitch()->lastError();
```

## Troubleshooting

### Kill Switch Not Working

**Problem**: Traffic not blocked when VPN disconnects

**Solutions**:
1. Verify application is running with Administrator privileges
2. Check Windows Firewall is enabled and running
3. Review logs for firewall operation errors
4. Manually check firewall rules: `netsh advfirewall firewall show rule name=all | findstr StartVPN`

### Internet Not Working After VPN Disconnect

**Problem**: No internet access after properly disconnecting VPN

**Solutions**:
1. Check if kill switch rules are still present:
   ```cmd
   netsh advfirewall firewall show rule name="StartVPN_KillSwitch_BlockOut"
   ```

2. Manually remove rules if necessary:
   ```cmd
   netsh advfirewall firewall delete rule name="StartVPN_KillSwitch_BlockOut"
   netsh advfirewall firewall delete rule name="StartVPN_KillSwitch_BlockIn"
   netsh advfirewall firewall delete rule name="StartVPN_KillSwitch_AllowVPNOut"
   netsh advfirewall firewall delete rule name="StartVPN_KillSwitch_AllowVPNIn"
   ```

3. Restart the application to ensure clean slate

### Performance Issues

**Problem**: Slow connection or high CPU usage

**Diagnosis**: Kill switch has minimal performance impact (<0.1% CPU)

**If issues persist**:
- Check for other firewall software conflicts
- Review Windows Firewall logs
- Disable other security software temporarily for testing

## Safety Features

### Automatic Cleanup
The kill switch includes several safety mechanisms:

1. **On Application Exit**: All firewall rules are automatically removed
2. **On Destructor Call**: Ensures cleanup even if app crashes
3. **Rule Conflict Detection**: Checks for existing rules before creating new ones
4. **Timeout Protection**: All netsh commands have 10-second timeout

### Logging
All kill switch operations are logged for troubleshooting:

```
- "Enabling kill switch..."
- "Kill switch enabled successfully"
- "Allowing traffic through VPN interface: VPN"
- "Disabling kill switch..."
- "Kill switch disabled successfully"
```

Check logs at: `C:\Users\[YourUser]\AppData\Local\StartVPN\logs\`

## Best Practices

### For Users
1. Always run StartVPN as Administrator
2. Keep Windows Firewall enabled
3. Don't manually modify StartVPN firewall rules
4. If you experience connectivity issues, restart the application

### For Developers
1. Test kill switch in a VM or test environment first
2. Always check return values from kill switch operations
3. Log all firewall operations for debugging
4. Test with different network configurations
5. Include kill switch status in bug reports

## Advanced Configuration

### Custom Interface Names
By default, the kill switch allows traffic through interfaces named "VPN". To customize:

```cpp
Start::Common::globalConnection()->killSwitch()->allowVPNInterface("Your-Interface-Name");
```

### Disable Kill Switch Feature
To disable kill switch functionality entirely (not recommended):

```cpp
// Don't call enableKillSwitch() in onStatusChanged()
// Or add a configuration option to skip it
```

## Security Considerations

### What Kill Switch Protects Against
✅ IP address leaks on VPN disconnect  
✅ Accidental traffic outside VPN tunnel  
✅ Application-level VPN bypasses  
✅ Unexpected VPN process crashes  

### What Kill Switch Does NOT Protect Against
❌ DNS leaks (requires separate DNS leak protection)  
❌ WebRTC leaks (requires browser-level protection)  
❌ IPv6 leaks (requires separate IPv6 leak protection)  
❌ Leaks before VPN connection establishes  

### Recommendations
1. Implement additional DNS leak protection
2. Consider IPv6 leak protection for complete coverage
3. Use browser extensions for WebRTC leak protection
4. Test regularly with leak detection tools

## Testing Your Kill Switch

### Manual Test Procedure
1. Connect to VPN through StartVPN
2. Verify your IP has changed (visit whatismyip.com)
3. Forcefully kill the VPN process:
   ```cmd
   taskkill /F /IM openvpn.exe
   ```
4. Try to access a website - it should be blocked
5. Reconnect VPN - access should be restored

### Automated Testing Script
```batch
@echo off
echo Testing Kill Switch...

echo 1. Checking current firewall rules...
netsh advfirewall firewall show rule name=all | findstr StartVPN

echo 2. Testing with VPN connected...
ping -n 1 google.com

echo 3. Killing VPN process...
taskkill /F /IM openvpn.exe

echo 4. Testing after VPN kill (should fail)...
ping -n 1 google.com

echo Test complete. Review results above.
pause
```

## FAQ

**Q: Will the kill switch work if I force-close the application?**  
A: Yes, but firewall rules may persist. The application attempts cleanup on exit, but forced termination may leave rules active. Manually remove them if needed.

**Q: Does kill switch affect LAN traffic?**  
A: No, local network traffic is not affected. Only internet-bound traffic is blocked.

**Q: Can I use other VPN software with StartVPN's kill switch active?**  
A: Not recommended. The firewall rules will block other VPN traffic. Disable kill switch first.

**Q: What happens if I lose Administrator privileges while running?**  
A: Kill switch operations will fail. The application will log errors but continue to function without kill switch protection.

**Q: How can I verify kill switch is working?**  
A: Check firewall rules with `netsh advfirewall firewall show rule name=all | findstr StartVPN` or use the manual test procedure above.

## Support

For issues related to kill switch functionality:
1. Check application logs
2. Review Windows Firewall settings
3. Test with Administrator privileges
4. Report issues with log files attached

Log Location: `%LOCALAPPDATA%\StartVPN\logs\`
