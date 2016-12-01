using CommandMessenger;

namespace G27PedalsAndShifterConfigurator
{
    public class DeviceStateParser
    {
        #region Exposed Methods

        public static DeviceState ParseState(ReceivedCommand cmd)
        {
            var state = new DeviceState();

            state.Pedals.Throttle = cmd.ReadInt16Arg();
            state.Pedals.Brake = cmd.ReadInt16Arg();
            state.Pedals.Clutch = cmd.ReadInt16Arg();

            for (var i = 0; i < 16; i++)
            {
                state.Shifter.Buttons[i] = cmd.ReadBoolArg();
            }

            state.Shifter.XPosition = cmd.ReadInt16Arg();
            state.Shifter.YPosition = cmd.ReadInt16Arg();

            return state;
        }

        #endregion
    }
}