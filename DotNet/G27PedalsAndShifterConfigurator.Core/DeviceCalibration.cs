namespace G27PedalsAndShifterConfigurator
{
    public class DeviceCalibration
    {
        #region Properties

        public PedalsCalibration Pedals { get; }

        public ShifterCalibration Shifter { get; set; }

        public bool HasAnyUnset => Pedals.HasAnyUnset && Shifter.HasAnyUnset;

        #endregion

        #region Constructors

        public DeviceCalibration()
        {
            Pedals = new PedalsCalibration();
            Shifter = new ShifterCalibration();
        }

        #endregion
    }

    public class PedalsCalibration
    {
        #region Properties

        public int MinThrottle { get; set; }
        public int MaxThrottle { get; set; }
        public int MinBrake { get; set; }
        public int MaxBrake { get; set; }
        public int MinClutch { get; set; }
        public int MaxClutch { get; set; }

        public bool HasAnyUnset =>
            MinThrottle < 0 || MaxThrottle < 1 || MinBrake < 0 || MaxBrake < 1 || MinClutch < 0 ||
            MaxClutch < 1;

        #endregion
    }

    public class ShifterCalibration
    {
        private int? _lowerY, _upperY;

        #region Properties

        public int Gate13 { get; set; }
        public int Gate24 { get; set; }
        public int Gate35 { get; set; }
        public int Gate46 { get; set; }

        public int MinY { get; set; }
        public int MaxY { get; set; }

        public int UpperY
        {
            get { return _upperY ?? (MaxY - MinY)/3; }
            set { _upperY = value; }
        }

        public int LowerY
        {
            get { return _lowerY ?? UpperY*2; }
            set { _lowerY = value; }
        }

        public bool HasAnyUnset => Gate13 < 1 || Gate24 < 1 || Gate35 < 1 || Gate46 < 1;

        #endregion
    }
}
