namespace G27PedalsAndShifterConfigurator
{
    public class DeviceCalibration
    {
        #region Properties

        public PedalsCalibration Pedals { get; set; }

        public ShifterCalibration Shifter { get; set; }

        public bool HasAnyUnset => Pedals.HasAnyUnset || Shifter.HasAnyUnset;

        public bool Changed => Pedals.Changed || Shifter.Changed;

        #endregion

        #region Constructors

        public DeviceCalibration() : this(new PedalsCalibration(), new ShifterCalibration()) {}

        public DeviceCalibration(PedalsCalibration pedals, ShifterCalibration shifter)
        {
            Pedals = pedals;
            Shifter = shifter;
        }

        #endregion

        #region Exposed Methods

        public static DeviceCalibration Reset()
        {
            return new DeviceCalibration(PedalsCalibration.Reset(), ShifterCalibration.Reset());
        }

        #endregion
    }

    public class PedalsCalibration
    {
        #region Private Members

        private bool _changed;
        private int _minThrottle;
        private int _maxThrottle;
        private int _minBrake;
        private int _maxBrake;
        private int _minClutch;
        private int _maxClutch;
        private bool _usePedals = true;
        private bool _invertBrake;

        #endregion

        #region Properties

        public bool Changed => _changed;

        public int MinThrottle
        {
            get { return _minThrottle; }
            set
            {
                _minThrottle = value;
                _changed = true;
            }
        }

        public int MaxThrottle
        {
            get { return _maxThrottle; }
            set
            {
                _maxThrottle = value;
                _changed = true;
            }
        }

        public int MinBrake
        {
            get { return _minBrake; }
            set
            {
                _minBrake = value;
                _changed = true;
            }
        }

        public int MaxBrake
        {
            get { return _maxBrake; }
            set
            {
                _maxBrake = value;
                _changed = true;
            }
        }

        public int MinClutch
        {
            get { return _minClutch; }
            set
            {
                _minClutch = value;
                _changed = true;
            }
        }

        public int MaxClutch
        {
            get { return _maxClutch; }
            set
            {
                _maxClutch = value;
                _changed = true;
            }
        }

        public bool UsePedals
        {
            get { return _usePedals; }
            set
            {
                if (_usePedals != value)
                {
                    _changed = true;
                }
                _usePedals = value;
            }
        }

        public bool InvertBrake
        {
            get { return _invertBrake; }
            set
            {
                if (_invertBrake != value)
                {
                    _changed = true;
                }
                _invertBrake = value;
            }
        }

        public bool HasAnyUnset =>
            MinThrottle < 0 || MaxThrottle < 1 || MinBrake < 0 || MaxBrake < 1 || MinClutch < 0 ||
            MaxClutch < 1;

        #endregion

        public PedalsCalibration() { }

        public PedalsCalibration(int minThrottle, int maxThrottle, int minBrake, int maxBrake, int minClutch,
            int maxClutch, bool usePedals, bool invertBrake)
        {
            _minThrottle = minThrottle;
            _maxThrottle = maxThrottle;
            _minBrake = minBrake;
            _maxBrake = maxBrake;
            _minClutch = minClutch;
            _maxClutch = maxClutch;
            _usePedals = usePedals;
            _invertBrake = invertBrake;
        }

        #region Exposed Methods

        public static PedalsCalibration Reset()
        {
            return new PedalsCalibration {
                MinThrottle = -1,
                MaxThrottle = -1,
                MinBrake = -1,
                MaxBrake = -1,
                MinClutch = -1,
                MaxClutch = -1
            };
        }

        #endregion
    }

    public class ShifterCalibration
    {
        #region Private Members

        private int? _lowerY, _upperY;
        private int _gate13;
        private bool _changed;
        private int _gate24;
        private int _gate35;
        private int _gate46;
        private int _minY;
        private int _maxY;
        private bool _useShifter = true;

        #endregion

        #region Properties

        public bool Changed => _changed;

        public int Gate13
        {
            get { return _gate13; }
            set
            {
                _gate13 = value;
                _changed = true;
            }
        }

        public int Gate24
        {
            get { return _gate24; }
            set
            {
                _gate24 = value;
                _changed = true;
            }
        }

        public int Gate35
        {
            get { return _gate35; }
            set
            {
                _gate35 = value;
                _changed = true;
            }
        }

        public int Gate46
        {
            get { return _gate46; }
            set
            {
                _gate46 = value;
                _changed = true;
            }
        }

        public int MinY
        {
            get { return _minY; }
            set
            {
                _minY = value;
                _changed = true;
            }
        }

        public int MaxY
        {
            get { return _maxY; }
            set
            {
                _maxY = value;
                _changed = true;
            }
        }

        public int UpperY
        {
            get { return _upperY ?? (MaxY - MinY)/3; }
            set
            {
                _upperY = value;
                _changed = true;
            }
        }

        public int LowerY
        {
            get { return _lowerY ?? UpperY*2; }
            set
            {
                _lowerY = value;
                _changed = true;
            }
        }

        public bool UseShifter
        {
            get { return _useShifter; }
            set
            {
                if (_useShifter != value)
                {
                    _changed = true;
                }
                _useShifter = value;
            }
        }

        public bool HasAnyUnset => Gate13 < 1 || Gate24 < 1 || Gate35 < 1 || Gate46 < 1;

        #endregion

        public ShifterCalibration() { }

        public ShifterCalibration(int gate13, int gate24, int gate35, int gate46, int lowerY, int upperY, bool useShifter)
        {
            _gate13 = gate13;
            _gate24 = gate24;
            _gate35 = gate35;
            _gate46 = gate46;
            _lowerY = lowerY;
            _upperY = upperY;
            _useShifter = useShifter;
        }

        #region Exposed Methods

        public static ShifterCalibration Reset()
        {
            return new ShifterCalibration {
                Gate13 = -1,
                Gate24 = -1,
                Gate35 = -1,
                Gate46 = -1,
                UpperY = -1,
                LowerY = -1
            };
        }

        #endregion
    }
}
