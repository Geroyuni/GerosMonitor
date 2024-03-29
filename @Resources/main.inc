[Rainmeter]
AccurateText = 1
Update = 50
DefaultUpdateDivider = 20
ContextTitle2 = Introduction/Help
ContextAction2 = [!ActivateConfig GerosMonitor\Help]
ContextTitle3 = Settings
ContextAction3 = [!ActivateConfig GerosMonitor\Settings]
ContextTitle4 = Restart
ContextAction4 = [!RefreshGroup Monitors]
ContextTitle5 = Manage Rainmeter
ContextAction5 = [!Manage Skins GerosMonitor]
ContextTitle6 = Unload
ContextAction6 = [!DeactivateConfigGroup Monitors]

[Variables]
@Include = vars.inc


; Styles aimed at meter type (rect, rectEX, graph, graphEX, text)
[StyleRect]
Shape = Path OuterSection | StrokeWidth 0 | Extend FillBorder
Shape2 = Path InnerSection
Shape3 = Path InnerSection | StrokeWidth 0 | Fill Color 0,0,0,1 |Scale #scale#,#scale#,-1,-1 | Extend FillBackground
Shape4 = Combine Shape | XOR Shape2 | Scale #scale#,#scale#,0,0
OuterSection = 12,0 | LineTo 1,0 | CurveTo 0,1,0.45,0,0,0.45 | LineTo 0,12 | CurveTo 1,13,0,12.55,0.45,13 | LineTo 12,13 | CurveTo 13,12,12.55,13,13,12.55 | LineTo 13,1 | CurveTo 12,0,13,0.45,12.55,0 | ClosePath 1
InnerSection = 12,12 | LineTo 1,12 | LineTo 1,1 | LineTo 12,1 | LineTo 12,12 | ClosePath 1

[StyleRectEX]
Shape = Path OuterSection | StrokeWidth 0 | Extend FillBorder
Shape2 = Path InnerSection
Shape3 = Path InnerSection | StrokeWidth 0 | Fill Color 0,0,0,1 | Scale #scale#,#scale#,-1,-1 | Extend FillBackground
Shape4 = Combine Shape | XOR Shape2 | Scale #scale#,#scale#,0,0
OuterSection = 52,0 | LineTo 1,0 | CurveTo 0,1,0.45,0,0,0.45 | LineTo 0,12 | CurveTo 1,13,0,12.55,0.45,13 | LineTo 52,13 | CurveTo 53,12,52.55,13,53,12.55 | LineTo 53,1 | CurveTo 52,0,53,0.45,52.55,0 | ClosePath 1
InnerSection = 52,12 | LineTo 1,12 | LineTo 1,1 | LineTo 52,1 | LineTo 52,12 | ClosePath 1
X = r
Y = r
Hidden = 1

[StyleGraph]
w = (Ceil(11 * #scale#))
h = (Ceil(11 * #scale#))
X = (1 * #scale#)r
Y = (1 * #scale#)r

[StyleGraphEX]
w = (Ceil(51 * #scale#))
h = (Ceil(11 * #scale#))
X = r
Y = r
Hidden = 1

[StyleText]
FontFace = Roboto
FontSize = (8 * #scale#)
FontCOlor = #colorTxt#
StringAlign = left
AntiAlias = 1
w = (35 * #scale#)
h = (8 * #scale#)
X = (16 * #scale#)r
Y = (-1 * #scale#)r


; Styles aimed at meter group (CPU, CPUtemp, GPU, GPUtemp, DSK, NET, RAM, FPS)
[StyleCPU]
Group = CPU
MeasureName = MeasureCPU
FillBorder = Fill Color #colorCPU#
PrimaryColor = #colorCPU#
Text = %1%

[StyleCPUtemp]
Group = CPUtemp
MeasureName = MeasureCPUtemp
FillBorder = Fill Color #colorCPU##colorCPUtempTransparency#
FIllBackground = Fill Color #colorCPUtempBackground#
PrimaryColor = #colorCPU##colorCPUtempTransparency#
FontColor = #colorCPUtempTxt##colorCPUtempTransparency#
Text = %1°#tempLetter#
DynamicVariables = 1

[StyleGPU]
Group = GPU
MeasureName = MeasureGPU
FillBorder = Fill Color #colorGPU##colorGPUtransparency#
PrimaryColor = #colorGPU##colorGPUtransparency#
FontColor = #colorTxt##colorGPUtransparency#
Text = %1%
DynamicVariables = 1

[StyleGPUtemp]
Group = GPUtemp
MeasureName = MeasureGPUtemp
FillBorder = Fill Color #colorGPU##colorGPUtempTransparency#
FIllBackground = Fill Color #colorGPUtempBackground#
PrimaryColor = #colorGPU##colorGPUtempTransparency#
FontColor = #colorGPUtempTxt##colorGPUtempTransparency#
Text = %1°#tempLetter#
DynamicVariables = 1

[StyleDSK]
Group = DSK
MeasureName = MeasureDSK
FillBorder = Fill Color #colorDSK#
PrimaryColor = #colorDSK#
Text = %1%

[StyleNET]
Group = NET
MeasureName = MeasureNET
FillBorder = Fill Color #colorNET#
PrimaryColor = #colorNET#

[StyleRAM]
Group = RAM
MeasureName = MeasureRAM
FillBorder = Fill Color #colorRAM#
PrimaryColor = #colorRAM#
Percentual = 1
Text = %1%

[StyleFPS]
Group = FPS
MeasureName = MeasureFPS
FillBorder = Fill Color #colorFPS##colorFPStransparency#
PrimaryColor = #colorFPS##colorFPStransparency#
FontColor = #colorTxt##colorFPStransparency#
DynamicVariables = 1


; Styles for coordinates (down, nextColumn, same)
[CoordDown]
X = (-17 * #scale#)r
Y = (17 * #scale#)r

[CoordNextColumn]
X = (40 * #scale#)r
Y = (-17 * #scale#)r

[CoordSame]
X = (-17 * #scale#)r
Y = r


; Measures (CPU, CPUtemp, GPU, GPUtemp, RAM, NET, FPS)
[MeasureCPUraw]
Measure = Plugin
Plugin = UsageMonitor
Category = "Processor Information"
Counter = "% Processor Utility"
Name = "0,_Total"

[MeasureCPU]
Measure = Calc
Formula = (MeasureCPUraw < 100) ? MeasureCPUraw : 100
MinValue = 0
MaxValue = 100

[MeasureCPUtemp]
Measure = Plugin
Plugin = MSIAfterburner.dll
DataSource = CPU temperature
MinValue = (#fahrenheit# = 1) ? 32 : 0
MaxValue = (#fahrenheit# = 1) ? 212 : 100
IfCondition = MeasureCPUtemp >= #tempHot#
IfTrueAction = [!SetVariable colorCPUtempTxt #colorHot#][!SetVariable colorCPUtempBackground #colorHot#64]
IfFalseAction = [!SetVariable colorCPUtempTxt #colorTxt#][!SetVariable colorCPUtempBackground 0,0,0,1]
IfCondition2 = MeasureCPUtemp = 0
IfTrueAction2 = [!SetVariable colorCPUtempTransparency 32]
IfFalseAction2 = [!SetVariable colorCPUtempTransparency ff]
IfCondition3 = MeasureCPUtemp >= 1000
IfTrueAction3 = [!SetOption MeterCPUtemp Text ∞°#tempLetter#]
IfFalseAction3 = [!SetOption MeterCPUtemp Text %1°#tempLetter#]

[MeasureGPUrawNumberless]
Measure = Plugin
Plugin = MSIAfterburner.dll
DataSource = GPU usage

[MeasureGPUrawNumbered]
Measure = Plugin
Plugin = MSIAfterburner.dll
DataSource = GPU#gpuNumber# usage

[MeasureGPUtempRawNumberless]
Measure = Plugin
Plugin = MSIAfterburner.dll
DataSource = GPU temperature

[MeasureGPUtempRawNumbered]
Measure = Plugin
Plugin = MSIAfterburner.dll
DataSource = GPU#gpuNumber# temperature

[MeasureGPU]
Measure = Calc
Formula = (MeasureGPUrawNumbered > 0) ? MeasureGPUrawNumbered : MeasureGPUrawNumberless
MinValue = 0
MaxValue = 100
IfCondition = (MeasureGPUtemp = 0) && (MeasureCPUtemp = 0)
IfTrueAction = [!SetVariable colorGPUtransparency 32]
IfFalseAction = [!SetVariable colorGPUtransparency ff]

[MeasureGPUtemp]
Measure = Calc
Formula = (MeasureGPUtempRawNumbered > 0) ? MeasureGPUtempRawNumbered : MeasureGPUtempRawNumberless
MinValue = (#fahrenheit# = 1) ? 32 : 0
MaxValue = (#fahrenheit# = 1) ? 212 : 100
IfCondition = MeasureGPUtemp >= #tempHot#
IfTrueAction = [!SetVariable colorGPUtempTxt #colorHot#][!SetVariable colorGPUtempBackground #colorHot#64]
IfFalseAction = [!SetVariable colorGPUtempTxt #colortxt#][!SetVariable colorGPUtempBackground 0,0,0,1]
IfCondition2 = MeasureGPUtemp = 0
IfTrueAction2 = [!SetVariable colorGPUtempTransparency 32]
IfFalseAction2 = [!SetVariable colorGPUtempTransparency ff]
IfCondition3 = MeasureGPUtemp >= 1000
IfTrueAction3 = [!SetOption MeterGPUtemp Text ∞°#tempLetter#]
IfFalseAction3 = [!SetOption MeterGPUtemp Text %1°#tempLetter#]

[MeasureDSKraw]
Measure = Plugin
Plugin = PerfMon
PerfMonObject = LogicalDisk
PerfMonCounter = % Disk Time
PerfMonInstance = #diskLetter#:

[MeasureDSK]
Measure = Calc
Formula = ((MeasureDSKraw / 100000) < 100) ? (MeasureDSKraw / 100000) : 100
MinValue = 0
MaxValue = 100

[MeasureNET]
Measure = NetTotal
Interface = Best
IfCondition = MeasureNET < 1000000
IfTrueAction = [!SetOption MeterNET NumOfDecimals 0][!SetOption MeterNET Scale 1000][!SetOption MeterNET Text %1kb]
IfCondition2 = (MeasureNET >= 1000000) && (MeasureNET < 10000000)
IfTrueAction2 = [!SetOption MeterNET NumOfDecimals 2][!SetOption MeterNET Scale 1000000][!SetOption MeterNET Text %1mb]
IfCondition3 = (MeasureNET >= 10000000) && (MeasureNET < 100000000)
IfTrueAction3 = [!SetOption MeterNET NumOfDecimals 1][!SetOption MeterNET Scale 1000000][!SetOption MeterNET Text %1mb]
IfCondition4 = (MeasureNET >= 100000000) && (MeasureNET < 1000000000)
IfTrueAction4 = [!SetOption MeterNET NumOfDecimals 0][!SetOption MeterNET Scale 1000000][!SetOption MeterNET Text %1mb]
IfCondition5 = MeasureNET >= 1000000000
IfTrueCondition5 = [!SetOption MeterNET NumOfDecimals 2][!SetOption MeterNET Scale 1000000000][!SetOption MeterNET Text %1gb]
UpdateDivider = 16
UseBits = 1

[MeasureRAM]
Measure = PhysicalMemory

[MeasureFPS]
Measure = Plugin
Plugin = MSIAfterburner.dll
DataSource = Framerate
IfCondition = MeasureFPS = 0
IfTrueAction = [!SetVariable colorFPStransparency 32][!SetOption MeterFPS Text ??fps]
IfFalseAction = [!SetVariable colorFPStransparency ff][!SetOption MeterFPS Text %1fps]


; Meters (for each group: rect, rectEX, graph, graphEX, text)
[MeterCPUrect]
Meter = Shape
MeterStyle = StyleCPU | StyleRect
X = #x#
Y = #y#
MouseOverAction = [!ToggleMeterGroup CPU]
MouseLeaveAction = [!ToggleMeterGroup CPU]

[MeterCPUrectEX]
Meter = Shape
MeterStyle = StyleCPU | StyleRectEX

[MeterCPUgraph]
Meter = Histogram
MeterStyle = StyleCPU | StyleGraph

[MeterCPUgraphEX]
Meter = Histogram
MeterStyle = StyleCPU | StyleGraphEX

[MeterCPU]
Meter = String
MeterStyle = StyleCPU | StyleText


[MeterCPUtempRect]
Meter = Shape
MeterStyle = StyleCPUtemp | StyleRect | CoordDown
MouseOverAction = [!ToggleMeterGroup CPUtemp]
MouseLeaveAction = [!ToggleMeterGroup CPUtemp]

[MeterCPUtempRectEX]
Meter = Shape
MeterStyle = StyleCPUtemp | StyleRectEX

[MeterCPUtempGraph]
Meter = Histogram
MeterStyle = StyleCPUtemp | StyleGraph

[MeterCPUtempGraphEX]
Meter = Histogram
MeterStyle = StyleCPUtemp | StyleGraphEX

[MeterCPUtemp]
Meter = String
MeterStyle = StyleText | StyleCPUtemp


[MeterGPUrect]
Meter = Shape
MeterStyle = StyleGPU | StyleRect | CoordNextColumn
MouseOverAction = [!ToggleMeterGroup GPU]
MouseLeaveAction = [!ToggleMeterGroup GPU]

[MeterGPUrectEX]
Meter = Shape
MeterStyle = StyleGPU | StyleRectEX

[MeterGPUgraph]
Meter = Histogram
MeterStyle = StyleGPU | StyleGraph

[MeterGPUgraphEX]
Meter = Histogram
MeterStyle = StyleGPU | StyleGraphEX

[MeterGPU]
Meter = String
MeterStyle = StyleText | StyleGPU


[MeterGPUtempRect]
Meter = Shape
MeterStyle = StyleGPUtemp | StyleRect | CoordDown
MouseOverAction = [!ToggleMeterGroup GPUtemp]
MouseLeaveAction = [!ToggleMeterGroup GPUtemp]

[MeterGPUtempRectEX]
Meter = Shape
MeterStyle = StyleGPUtemp | StyleRectEX

[MeterGPUtempGraph]
Meter = Histogram
MeterStyle = StyleGPUtemp | StyleGraph

[MeterGPUtempGraphEX]
Meter = Histogram
MeterStyle = StyleGPUtemp | StyleGraphEX

[MeterGPUtemp]
Meter = String
MeterStyle = StyleText | StyleGPUtemp


[MeterDSKrect]
Meter = Shape
MeterStyle = StyleDSK | StyleRect | CoordNextColumn
MouseOverAction = [!ToggleMeterGroup DSK]
MouseLeaveAction = [!ToggleMeterGroup DSK]

[MeterDSKrectEX]
Meter = Shape
MeterStyle = StyleDSK | StyleRectEX

[MeterDSKgraph]
Meter = Histogram
MeterStyle = StyleDSK | StyleGraph

[MeterDSKgraphEX]
Meter = Histogram
MeterStyle = StyleDSK | StyleGraphEX

[MeterDSK]
Meter = String
MeterStyle = StyleDSK | StyleText


[MeterNETrect]
Meter = Shape
MeterStyle = StyleNET | StyleRect | CoordDown
MouseOverAction = [!ToggleMeterGroup NET]
MouseLeaveAction = [!ToggleMeterGroup NET]

[MeterNETrectEX]
Meter = Shape
MeterStyle = StyleNET | StyleRectEX

[MeterNETgraph]
Meter = Histogram
MeterStyle = StyleNET | StyleGraph
AutoScale = 1

[MeterNETgraphEX]
Meter = Histogram
MeterStyle = StyleNET | StyleGraphEX
AutoScale = 1

[MeterNET]
Meter = String
MeterStyle = StyleNET | StyleText


[MeterRAMrect]
Meter = Shape
MeterStyle = StyleRAM | StyleRect | CoordNextColumn
MouseOverAction = [!ToggleMeterGroup RAM]
MouseLeaveAction = [!ToggleMeterGroup RAM]

[MeterRAMrectEX]
Meter = Shape
MeterStyle = StyleRAM | StyleRectEX

[MeterRAMgraph]
Meter = Histogram
MeterStyle = StyleRAM | StyleGraph

[MeterRAMgraphEX]
Meter = Histogram
MeterStyle = StyleRAM | StyleGraphEX

[MeterRAM]
Meter = String
MeterStyle = StyleRAM | StyleText


[MeterFPSrect]
Meter = Shape
MeterStyle = StyleFPS | StyleRect | CoordDown
MouseOverAction = [!ToggleMeterGroup FPS]
MouseLeaveAction = [!ToggleMeterGroup FPS]

[MeterFPSrectEX]
Meter = Shape
MeterStyle = StyleFPS | StyleRectEX

[MeterFPSgraph]
Meter = Histogram
MeterStyle = StyleFPS | StyleGraph
AutoScale = 1

[MeterFPSgraphEX]
Meter = Histogram
MeterStyle = StyleFPS | StyleGraphEX
AutoScale = 1

[MeterFPS]
Meter = String
MeterStyle = StyleText | StyleFPS
