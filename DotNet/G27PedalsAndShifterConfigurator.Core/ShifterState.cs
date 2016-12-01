namespace G27PedalsAndShifterConfigurator
{
    public class ShifterState
    {
        #region Constants

        public const int BUTTON_COUNT = 16;

        #endregion

        #region Properties

        public bool[] Buttons { get; protected set; }
        public int XPosition { get; set; }
        public int YPosition { get; set; }

        #endregion

        #region Constructors

        public ShifterState()
        {
            Buttons = new bool[BUTTON_COUNT];
        }

        #endregion
    }
}