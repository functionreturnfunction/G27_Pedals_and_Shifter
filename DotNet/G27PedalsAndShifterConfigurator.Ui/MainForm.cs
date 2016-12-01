using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Timer = System.Timers.Timer;

namespace G27PedalsAndShifterConfigurator
{
    public partial class MainForm : Form
    {
        protected UsbDeviceHelper _usbHelper;
        protected Timer _statusTimer;

        public int ComPort => (int)nudComPort.Value;

        public MainForm()
        {
            InitializeComponent();
            FixColors();
        }

        private void FixColors()
        {
            pgActualClutch.ColorBrush = Brushes.Blue;
        }

        private void btnConnectDevice_Click(object sender, EventArgs e)
        {
            string message;
            if (!ConnectDeviceValidator.IsValid(this, out message))
            {
                MessageBox.Show(message);
                return;
            }

            _usbHelper = new UsbDeviceHelper(ComPort,
                s => rtbDeviceOutput.Invoke(new EventHandler(delegate { rtbDeviceOutput.Text = s?.ToString() ?? "Error getting device status"; })));

            btnDisconnectDevice.Visible = true;
            btnConnectDevice.Visible = false;

            InitTimer();
        }

        private void InitTimer()
        {
            _statusTimer = new Timer {Interval = 50};
            _statusTimer.Elapsed += (sender, args) => _usbHelper.DisplayStatus();
            _statusTimer.Enabled = true;
        }

        private void btnDisconnectDevice_Click(object sender, EventArgs e)
        {
            _usbHelper.Disconnect();
            _usbHelper.Dispose();
            _statusTimer.Enabled = false;
            _statusTimer.Dispose();

            btnDisconnectDevice.Visible = false;
            btnConnectDevice.Visible = true;
        }
    }
}
