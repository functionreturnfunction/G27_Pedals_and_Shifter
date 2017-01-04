using CommandMessenger;

namespace G27PedalsAndShifterConfigurator
{
    public class DeviceCalibrationParser
    {
        public static DeviceCalibration ParseCalibration(ReceivedCommand cmd)
        {
            return
                new DeviceCalibration(
                    new PedalsCalibration(minThrottle: cmd.ReadInt16Arg(), maxThrottle: cmd.ReadInt16Arg(),
                        minBrake: cmd.ReadInt16Arg(), maxBrake: cmd.ReadInt16Arg(), minClutch: cmd.ReadInt16Arg(),
                        maxClutch: cmd.ReadInt16Arg(), usePedals: cmd.ReadBoolArg(), invertBrake: cmd.ReadBoolArg()),
                    new ShifterCalibration(gate13: cmd.ReadInt16Arg(), gate24: cmd.ReadInt16Arg(),
                        gate35: cmd.ReadInt16Arg(), gate46: cmd.ReadInt16Arg(), lowerY: cmd.ReadInt16Arg(),
                        upperY: cmd.ReadInt16Arg(), useShifter: cmd.ReadBoolArg()));
        }
    }
}