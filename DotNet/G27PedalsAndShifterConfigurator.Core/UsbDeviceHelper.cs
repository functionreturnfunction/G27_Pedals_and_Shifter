using System;
using CommandMessenger;
using CommandMessenger.Transport.Serial;

namespace G27PedalsAndShifterConfigurator
{
    public class UsbDeviceHelper : IDisposable
    {
        #region Constants

        public const int BAUD = 115200;
        public const int DEFAULT_TIMEOUT = 1000;

        #endregion

        #region Enums

        public enum Command
        {
            Acknowledge,
            Error,
            GetStatus,
            GetStatusResult,
            GetCalibration,
            GetCalibrationResult,
            SetCalibration,
            SetCalibrationResult
        }

        #endregion

        #region Private Members

        private readonly SerialTransport _serialTransport;
        private readonly CmdMessenger _commandMessenger;
        private readonly Action<DeviceState> _displayStatusFn;

        #endregion

        public bool IsConncected => _serialTransport.IsConnected();

        #region Constructors

        public UsbDeviceHelper(string port, Action<DeviceState> displayStatusFn)
        {
            _displayStatusFn = displayStatusFn;
            _serialTransport = new SerialTransport {
                CurrentSerialSettings = {PortName = port, BaudRate = BAUD, DtrEnable = true}
            };

            _commandMessenger = new CmdMessenger(_serialTransport);
            _commandMessenger.Connect();

        }

        public UsbDeviceHelper(int port, Action<DeviceState> displayStatusFn) : this($"COM{port}", displayStatusFn) {}

        #endregion

        #region Exposed Methods

        public void DisplayStatus()
        {
            _displayStatusFn(GetStatus());
        }

        public DeviceState GetStatus()
        {
            var command = new SendCommand((int)Command.GetStatus, (int)Command.GetStatusResult, DEFAULT_TIMEOUT);

            try
            {
                var result = _commandMessenger.SendCommand(command);

                return result.Ok ? DeviceStateParser.ParseState(result) : default(DeviceState);
            }
            catch
            {
                return null;
            }
        }

        public void SetCalibration(DeviceCalibration calibration)
        {
            var cmd = new SendCommand((int)Command.SetCalibration, (int)Command.SetCalibrationResult, DEFAULT_TIMEOUT);

            cmd.AddArgument(calibration.Pedals.MinThrottle);
            cmd.AddArgument(calibration.Pedals.MaxThrottle);
            cmd.AddArgument(calibration.Pedals.MinBrake);
            cmd.AddArgument(calibration.Pedals.MaxBrake);
            cmd.AddArgument(calibration.Pedals.MinClutch);
            cmd.AddArgument(calibration.Pedals.MaxClutch);
            cmd.AddArgument(calibration.Shifter.Gate13);
            cmd.AddArgument(calibration.Shifter.Gate24);
            cmd.AddArgument(calibration.Shifter.Gate35);
            cmd.AddArgument(calibration.Shifter.Gate46);
            cmd.AddArgument(calibration.Shifter.LowerY);
            cmd.AddArgument(calibration.Shifter.UpperY);

            _commandMessenger.SendCommand(cmd);
        }

        public DeviceCalibration GetCalibration()
        {
            var command = new SendCommand((int)Command.GetCalibration, (int)Command.GetCalibrationResult, DEFAULT_TIMEOUT);

            var result = _commandMessenger.SendCommand(command);

            return result.Ok ? DeviceCalibrationParser.ParseCalibration(result) : default(DeviceCalibration);
        }

        public void Disconnect()
        {
            _commandMessenger.Disconnect();
            _serialTransport.Disconnect();
        }

        public void Dispose()
        {
            _commandMessenger.Dispose();
            _serialTransport.Dispose();
        }

        #endregion
    }
}