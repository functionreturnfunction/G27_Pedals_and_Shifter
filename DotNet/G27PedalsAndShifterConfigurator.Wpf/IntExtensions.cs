namespace G27PedalsAndShifterConfigurator.Wpf
{
    public static class IntExtensions
    {
        public static bool Between(this int that, int lesser, int greater)
        {
            return lesser <= that && greater >= that;
        }
    }
}