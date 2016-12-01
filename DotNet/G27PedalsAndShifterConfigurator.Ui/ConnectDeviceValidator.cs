using System.IO.Ports;
using System.Linq;

namespace G27PedalsAndShifterConfigurator
{
    public class ConnectDeviceValidator
    {
        public static bool IsValid(MainForm form, out string message)
        {
            if (form.ComPort <= 0)
            {
                message = "ComPort must be a positive value.";
                return false;
            }

            message = null;
            return true;
        }
    }
}