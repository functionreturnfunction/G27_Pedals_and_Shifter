namespace G27PedalsAndShifterConfigurator.Wpf
{
    public static class ConnectDeviceValidator
    {
        public static bool IsValid(MainWindow wnd, out string message)
        {
            if (string.IsNullOrWhiteSpace(wnd.ComPort))
            {
                message = "Please select a COM Port.";
                return false;
            }

            message = null;
            return true;
        }
    }
}