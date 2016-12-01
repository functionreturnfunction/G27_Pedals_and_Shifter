using System.Drawing;
using System.Windows.Forms;

namespace G27PedalsAndShifterConfigurator
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lblComPort = new System.Windows.Forms.Label();
            this.btnDisconnectDevice = new System.Windows.Forms.Button();
            this.btnConnectDevice = new System.Windows.Forms.Button();
            this.nudComPort = new System.Windows.Forms.NumericUpDown();
            this.rtbDeviceOutput = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.pgVirtualThrottle = new G27PedalsAndShifterConfigurator.VerticalProgressBar();
            this.pgActualThrottle = new G27PedalsAndShifterConfigurator.VerticalProgressBar();
            this.pgVirtualBrake = new G27PedalsAndShifterConfigurator.VerticalProgressBar();
            this.pgActualBrake = new G27PedalsAndShifterConfigurator.VerticalProgressBar();
            this.pgVirtualClutch = new G27PedalsAndShifterConfigurator.VerticalProgressBar();
            this.pgActualClutch = new G27PedalsAndShifterConfigurator.VerticalProgressBar();
            ((System.ComponentModel.ISupportInitialize)(this.nudComPort)).BeginInit();
            this.SuspendLayout();
            // 
            // lblComPort
            // 
            this.lblComPort.AutoSize = true;
            this.lblComPort.Location = new System.Drawing.Point(11, 15);
            this.lblComPort.Name = "lblComPort";
            this.lblComPort.Size = new System.Drawing.Size(56, 13);
            this.lblComPort.TabIndex = 4;
            this.lblComPort.Text = "COM Port:";
            // 
            // btnDisconnectDevice
            // 
            this.btnDisconnectDevice.Location = new System.Drawing.Point(179, 10);
            this.btnDisconnectDevice.Name = "btnDisconnectDevice";
            this.btnDisconnectDevice.Size = new System.Drawing.Size(127, 23);
            this.btnDisconnectDevice.TabIndex = 7;
            this.btnDisconnectDevice.Text = "Disconnect Device";
            this.btnDisconnectDevice.UseVisualStyleBackColor = true;
            this.btnDisconnectDevice.Visible = false;
            this.btnDisconnectDevice.Click += new System.EventHandler(this.btnDisconnectDevice_Click);
            // 
            // btnConnectDevice
            // 
            this.btnConnectDevice.Location = new System.Drawing.Point(179, 10);
            this.btnConnectDevice.Name = "btnConnectDevice";
            this.btnConnectDevice.Size = new System.Drawing.Size(127, 23);
            this.btnConnectDevice.TabIndex = 7;
            this.btnConnectDevice.Text = "Connect Device";
            this.btnConnectDevice.UseVisualStyleBackColor = true;
            this.btnConnectDevice.Click += new System.EventHandler(this.btnConnectDevice_Click);
            // 
            // nudComPort
            // 
            this.nudComPort.Location = new System.Drawing.Point(73, 12);
            this.nudComPort.Name = "nudComPort";
            this.nudComPort.Size = new System.Drawing.Size(100, 20);
            this.nudComPort.TabIndex = 9;
            // 
            // rtbDeviceOutput
            // 
            this.rtbDeviceOutput.Location = new System.Drawing.Point(14, 38);
            this.rtbDeviceOutput.Name = "rtbDeviceOutput";
            this.rtbDeviceOutput.ReadOnly = true;
            this.rtbDeviceOutput.Size = new System.Drawing.Size(292, 211);
            this.rtbDeviceOutput.TabIndex = 10;
            this.rtbDeviceOutput.Text = "Waitng for device connection...";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 353);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 13);
            this.label1.TabIndex = 17;
            this.label1.Text = "Clutch";
            this.label1.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(64, 353);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 18;
            this.label2.Text = "Brake";
            this.label2.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(108, 353);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(43, 13);
            this.label3.TabIndex = 19;
            this.label3.Text = "Throttle";
            this.label3.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // pgVirtualThrottle
            // 
            this.pgVirtualThrottle.Location = new System.Drawing.Point(132, 255);
            this.pgVirtualThrottle.Name = "pgVirtualThrottle";
            this.pgVirtualThrottle.Size = new System.Drawing.Size(15, 95);
            this.pgVirtualThrottle.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.pgVirtualThrottle.TabIndex = 16;
            this.pgVirtualThrottle.Value = 50;
            // 
            // pgActualThrottle
            // 
            this.pgActualThrottle.Location = new System.Drawing.Point(111, 255);
            this.pgActualThrottle.Name = "pgActualThrottle";
            this.pgActualThrottle.Size = new System.Drawing.Size(15, 95);
            this.pgActualThrottle.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.pgActualThrottle.TabIndex = 15;
            this.pgActualThrottle.Value = 50;
            // 
            // pgVirtualBrake
            // 
            this.pgVirtualBrake.Location = new System.Drawing.Point(84, 255);
            this.pgVirtualBrake.Name = "pgVirtualBrake";
            this.pgVirtualBrake.Size = new System.Drawing.Size(15, 95);
            this.pgVirtualBrake.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.pgVirtualBrake.TabIndex = 14;
            this.pgVirtualBrake.Value = 50;
            // 
            // pgActualBrake
            // 
            this.pgActualBrake.Location = new System.Drawing.Point(63, 255);
            this.pgActualBrake.Name = "pgActualBrake";
            this.pgActualBrake.Size = new System.Drawing.Size(15, 95);
            this.pgActualBrake.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.pgActualBrake.TabIndex = 13;
            this.pgActualBrake.Value = 50;
            // 
            // pgVirtualClutch
            // 
            this.pgVirtualClutch.Location = new System.Drawing.Point(35, 255);
            this.pgVirtualClutch.Name = "pgVirtualClutch";
            this.pgVirtualClutch.Size = new System.Drawing.Size(15, 95);
            this.pgVirtualClutch.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.pgVirtualClutch.TabIndex = 12;
            this.pgVirtualClutch.Value = 50;
            // 
            // pgActualClutch
            // 
            this.pgActualClutch.Location = new System.Drawing.Point(14, 255);
            this.pgActualClutch.Name = "pgActualClutch";
            this.pgActualClutch.Size = new System.Drawing.Size(15, 95);
            this.pgActualClutch.TabIndex = 11;
            this.pgActualClutch.Value = 50;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(316, 471);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pgVirtualThrottle);
            this.Controls.Add(this.pgActualThrottle);
            this.Controls.Add(this.pgVirtualBrake);
            this.Controls.Add(this.pgActualBrake);
            this.Controls.Add(this.pgVirtualClutch);
            this.Controls.Add(this.pgActualClutch);
            this.Controls.Add(this.rtbDeviceOutput);
            this.Controls.Add(this.nudComPort);
            this.Controls.Add(this.btnDisconnectDevice);
            this.Controls.Add(this.btnConnectDevice);
            this.Controls.Add(this.lblComPort);
            this.Name = "MainForm";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.nudComPort)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label lblComPort;
        private System.Windows.Forms.Button btnDisconnectDevice;
        private System.Windows.Forms.Button btnConnectDevice;
        private System.Windows.Forms.NumericUpDown nudComPort;
        private System.Windows.Forms.RichTextBox rtbDeviceOutput;
        private VerticalProgressBar pgActualClutch;
        private VerticalProgressBar pgVirtualClutch;
        private VerticalProgressBar pgVirtualBrake;
        private VerticalProgressBar pgActualBrake;
        private VerticalProgressBar pgVirtualThrottle;
        private VerticalProgressBar pgActualThrottle;
        private Label label1;
        private Label label2;
        private Label label3;
    }

    public class MyProgressBar : ProgressBar
    {
        private Brush _colorBrush = Brushes.Green;

        public Brush ColorBrush
        {
            set { _colorBrush = value; }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            var rec = e.ClipRectangle;

            rec.Width = (int)(rec.Width*((double)Value/Maximum)) - 4;
            if (ProgressBarRenderer.IsSupported)
            {
                ProgressBarRenderer.DrawHorizontalBar(e.Graphics, e.ClipRectangle);
            }
            rec.Height = rec.Height - 4;
            e.Graphics.FillRectangle(_colorBrush, 2, 2, rec.Width, rec.Height);
        }
    }

    public class VerticalProgressBar : MyProgressBar
    {
        protected override CreateParams CreateParams
        {
            get
            {
                var baseP = base.CreateParams;
                baseP.Style |= 0x04;
                return baseP;
            }
        }
    }
}

