import sys
import struct
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from matplotlib import gridspec
from matplotlib.widgets import RadioButtons, CheckButtons, Button
import serial

class Plotter:
    def __init__(self):
        self.fig = plt.figure("G27 Pedals and Shifter", figsize=(10,8)) 
        gs = gridspec.GridSpec(4, 2, width_ratios=[1, 1], height_ratios=[3,1,0.3,0.3]) 
        self.pedals = self.fig.add_subplot(gs[0])
        self.shifter = self.fig.add_subplot(gs[1])
        self.modes = RadioButtons(self.fig.add_subplot(gs[2]), ["Idle", "Shifter neutral zone", "Shifter 135 zone", "Shifter 246R zone", "Shifter 12 zone", "Shifter 35 zone", "Gas pedal (if not auto-calibrated)", "Brake pedal (if not auto-calibrated)", "Clutch pedal (if not auto-calibrated)"])
        self.optionlist = ["Enable auto-calibration", "Invert brake pedal", "Enable pedals", "Enable shifter"]
        self.options = CheckButtons(self.fig.add_subplot(gs[3]), self.optionlist)
        self.btnSave = Button(self.fig.add_subplot(gs[4]), 'Save Calib to EEPROM')
        self.btnResetDefault = Button(self.fig.add_subplot(gs[5]), 'Reset Calib to default')
        self.btnResetEEPROM = Button(self.fig.add_subplot(gs[7]), 'Reset Calib to EEPROM')
        
        self.pp = self.pedals.bar(['gas','brake','clutch'], height=1023)
        self.pedals.set_ylim([0, 1023])
        self.pedals.set_title("Pedals")
        self.sp, = self.shifter.plot([0],[0], 'x')
        self.nz = Rectangle( (0,450), 1024, 100, facecolor=(0.9,0.9,0.9, 0.5), edgecolor='b' )
        self.g135 = Rectangle( (0,450), 1024, 100, facecolor=(0.9,0.9,0.9, 0.5), edgecolor='r' )
        self.g246 = Rectangle( (0,450), 1024, 100, facecolor=(0.9,0.9,0.9, 0.5), edgecolor='g' )
        self.g12 = Rectangle( (0,0), 200, 1024, facecolor=(0.9,0.9,0.9, 0.5), edgecolor='m' )
        self.g56 = Rectangle( (823,0), 200, 1024, facecolor=(0.9,0.9,0.9, 0.5), edgecolor='y' )
        self.shifter.add_patch(self.nz)
        self.shifter.add_patch(self.g135)
        self.shifter.add_patch(self.g246)
        self.shifter.add_patch(self.g12)
        self.shifter.add_patch(self.g56)
        self.shifter.set_xlim([0, 1023])
        self.shifter.set_ylim([0, 1023])
        self.shifter.set_title("Shifter Position")

    def apply(self, 
              sx, sy, g, b, c, g135, g246, n0, n1, g12, g56, gas0, gas1, brake0, brake1, clutch0, clutch1, 
              pedals_auto_calib, enable_pedals, enable_shifter, invert_brake, *args):
        for i, ppp in enumerate(self.pp):
            ppp.set_height([g,b,c][i])
        self.sp.set_xdata([sx])
        self.sp.set_ydata([sy])
        self.nz.set_y(n0)
        self.nz.set_height(n1-n0)
        self.g135.set_y(g135)
        self.g135.set_height(1024-g135)
        self.g246.set_y(0)
        self.g246.set_height(g246)
        self.g12.set_x(0)
        self.g12.set_width(g12)
        self.g56.set_x(g56)
        self.g56.set_width(1024-g56)

        for o,v in [("Enable auto-calibration", pedals_auto_calib),
                    ("Invert brake pedal", invert_brake),
                    ("Enable pedals", enable_pedals),
                    ("Enable shifter", enable_shifter)]:
            if int(self.options.get_status()[self.optionlist.index(o)]) != int(v):
                self.options.set_active(self.optionlist.index(o))
        
        self.fig.canvas.draw()
        self.fig.canvas.flush_events()
        
def main():
    tty = sys.argv[1] if len(sys.argv) > 1 else "/dev/ttyACM0"
    p = Plotter()
    p.fig.show()
    with serial.Serial(tty, 115200, timeout=1) as ser:
        name_to_mode = {
            "Idle": b'i', 
            "Shifter neutral zone": b'n', 
            "Shifter 135 zone": b'u', 
            "Shifter 246R zone": b'b', 
            "Shifter 12 zone": b'l', 
            "Shifter 35 zone": b'r',
            "Gas pedal (if not auto-calibrated)": b"G", 
            "Brake pedal (if not auto-calibrated)": b"B", 
            "Clutch pedal (if not auto-calibrated)": b"C",
            }
        p.modes.on_clicked(lambda name: ser.write(name_to_mode[name]))
        option_to_cmd = {
            "Enable auto-calibration" : (b"p", b"P"), 
            "Invert brake pedal" : (b"x", b"X"),
            "Enable pedals" : (b"e", b"E"), 
            "Enable shifter" : (b"s", b"S"),
            }
        p.options.on_clicked(lambda name: ser.write(option_to_cmd[name][int(p.options.get_status()[p.optionlist.index(name)])]))
        p.btnSave.on_clicked(lambda *args: [print("Saving calibration."), ser.write(b'w')])
        p.btnResetDefault.on_clicked(lambda *args: [print("Reset to default."), ser.write(b'c')])
        p.btnResetEEPROM.on_clicked(lambda *args: [print("Reset to EEPROM."), ser.write(b'U')])
        ser.write(b'O') # enable serial output
        finished = False
        def close(*args):
            nonlocal finished
            finished = True
        p.fig.canvas.mpl_connect('close_event', close)
        while not finished:
            l = ser.readline().strip()
            items = [int(i) for i in l.split(b" ")]
            p.apply(*items[1:])
        ser.write(b'o') # disable serial output
        
if __name__ == "__main__":
    main()
