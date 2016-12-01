namespace G27PedalsAndShifterConfigurator
{
    public class DeviceCalibration
    {
        #region Properties

        public PedalsCalibration Pedals { get; }

        public ShifterCalibration Shifter { get; set; }

        public bool IsAllZeroes => Pedals.IsAllZeroes && Shifter.IsAllZeroes;

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

        public bool IsAllZeroes =>
            MinThrottle == MaxThrottle && MaxThrottle == MinBrake && MinBrake == MaxBrake && MaxBrake == MinClutch &&
            MinClutch == MaxClutch && MaxClutch == 0;

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

        public bool IsAllZeroes => Gate13 == Gate24 && Gate24 == Gate35 && Gate35 == Gate46 && Gate46 == 0;

        #endregion
    }
}
