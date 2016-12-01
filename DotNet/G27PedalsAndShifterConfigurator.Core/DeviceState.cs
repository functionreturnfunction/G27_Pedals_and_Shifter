using System.Text;

namespace G27PedalsAndShifterConfigurator
{
    public class DeviceState
    {
        #region Private Members

        #endregion

        #region Properties

        public PedalsState Pedals { get; }

        public ShifterState Shifter { get; }

        #endregion

        #region Constructors

        public DeviceState()
        {
            Pedals = new PedalsState();
            Shifter = new ShifterState();
        }

        #endregion

        #region Exposed Methods

        public override string ToString()
        {
            var sb = new StringBuilder();

            sb.Append($"THROTTLE: {Pedals.Throttle} ");
            sb.Append($"BRAKE: {Pedals.Brake} ");
            sb.Append($"CLUTCH: {Pedals.Clutch} ");

            for (var i = 0; i < 16; i++)
            {
                sb.Append($"BUTTON {i}: {Shifter.Buttons[i]} ");
            }

            sb.Append($"SHIFTER X: {Shifter.XPosition} ");
            sb.Append($"SHIFTER Y: {Shifter.YPosition}");

            return sb.ToString();
        }

        #endregion
    }
}