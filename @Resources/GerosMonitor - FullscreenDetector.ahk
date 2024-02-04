#Requires AutoHotkey v2.0
#SingleInstance force
#NoTrayIcon
DetectHiddenWindows True

isFullscreen(previousFullscreenState, windowTitle) {
    try {
        currentMonitor := getCurrentMonitor(windowTitle)
        monitor := MonitorGet(currentMonitor, &monLeft, &monTop, &monRight, &monBottom)
        classWindow := WinGetClass("A")

        ; Based off superuser.com/a/656145
        barData := Buffer(A_PtrSize=4 ? 36:48)
        NumPut("UInt", A_PtrSize=4 ? 36:48, barData)
        autoHideTaskbarEnabled := DllCall("Shell32\SHAppBarMessage"
            , "UInt", 4, "Ptr", barData, "Int")

        if autoHideTaskbarEnabled {
            ; Allow user to move the skin without hiding the skin on them
            if classWindow = "RainmeterMeterWindow" and GetKeyState("LButton")
                return previousFullscreenState

            currentTaskbar := getCurrentTaskbar(currentMonitor)
            WinGetPos(, &yTaskbar, , &hTaskbar, "ahk_ID " . currentTaskbar)
            taskbarHiddenBottom := (yTaskbar > (monBottom - hTaskbar))
            taskbarHiddenTop := yTaskbar and (yTaskbar < 0)

            debugMsgBox("Debug message from holding Ctrl+Shift+Alt+Comma."
                . "`nScreen " . monLeft . " " . monTop . " " . monRight . " "
                . monBottom . " (display " . currentMonitor . ")"
                . "`nTaskbar " . yTaskbar . "y " . hTaskbar . "h "
                . "`ntaskbarHiddenBottom " . taskbarHiddenBottom
                . "`ntaskbarHiddenTop " . taskbarHiddenTop)

            return taskbarHiddenBottom or taskbarHiddenTop
        }

        WinGetPos(&xWindow, &yWindow, &wWindow, &hWindow, "A")

        appInCurrentDisplay := (xWindow >= monLeft - 15)
            and (xWindow <= monRight - 15)
            and (yWindow >= monTop - 15)
            and (yWindow <= monBottom - 15)
        appBiggerThanCurrentDisplay := (wWindow >= (monRight - monLeft))
            and (hWindow >= (monBottom - monTop))

        ; 1. Ignore two Windows OS programs that use the entire screen
        ; 2. Ignore raimeter window, as when clicked it can swap modes
        ; 3. Ignore windows from different displays
        shouldIgnore := InStr("WorkerW|Progman", classWindow)
            or classWindow = "RainmeterMeterWindow"
            or not appInCurrentDisplay

        debugMsgBox("Debug message from holding Ctrl+Shift+Alt+Comma."
            . "`nScreen " . monLeft . " " . monTop . " " . monRight
            . " " . monBottom . " (display " . currentMonitor . ")"
            . "`nActive window " . xWindow . "x " . yWindow "y "
            . wWindow . "w " . hWindow . "h " . classWindow
            . "`nappInCurrentDisplay " . appInCurrentDisplay
            . "`nappBiggerThanCurrentDisplay " . appBiggerThanCurrentDisplay)

        if shouldIgnore
            return previousFullscreenState

        return appBiggerThanCurrentDisplay
    }
    catch
        return previousFullscreenState
}

debugMsgBox(message) {
    Loop 4 {
        if not (GetKeyState("Ctrl") and GetKeyState("Shift") and GetKeyState("Alt") and GetKeyState(","))
            return
        Sleep(200)
    }

    MsgBox(message)
}

getCurrentMonitor(windowTitle) {
    If WinExist(windowTitle) {
        WinGetPos(&winX, &winY, &winW, &winH)

        Loop MonitorGetCount() {
            MonitorGet(A_Index, &monLeft, &monTop, &monRight, &monBottom)

            isInsideMonitor := (winX >= monLeft
                and winX < monRight
                and winY >= monTop
                and winY < monBottom)

            if isInsideMonitor
                return A_Index
        }
    }

    return MonitorGetPrimary()
}

getCurrentTaskbar(currentMonitor) {
    if currentMonitor != MonitorGetPrimary() {
        windowList := WinGetList("ahk_class Shell_SecondaryTrayWnd")
        monitor := MonitorGet(currentMonitor, &monLeft, &monTop, &monRight, &monBottom)

        for _, winID in windowList {
            WinGetPos(&winX, &winY, &winW, &winH, "ahk_id " . winID)

            isInsideMonitor := (winX >= monLeft
                and winX < monRight
                and winY >= monTop
                and winY < monBottom)

            if isInsideMonitor
                return winID
        }
    }

    return WinGetID("ahk_class Shell_TrayWnd")
}

getScale(previousScale, windowTitle) {
    try {
        If !WinExist(windowTitle)
            return previousScale

        hwnd := WinGetID()
        dpi := DllCall("User32\GetDpiForWindow", "Ptr", hwnd, "UInt")

        if dpi = 0
            return previousScale

        return (dpi / 96)  ; 96 is 1x scale, double (192) is 2x scale
    }
    catch
        return previousScale

}

isFullscreenAction(rainmeterPath, hideSkins) {
    if hideskins = 1
        Run(rainmeterPath . " [!HideFade GerosMonitor\Taskbar][!HideFade GerosMonitor\Portable]")
    else
        Run(rainmeterPath . " [!HideFade GerosMonitor\Taskbar][!ShowFade GerosMonitor\Portable]")
}

isNotFullscreenAction(rainmeterPath, hideSkins) {
    if hideskins = 2
        Run(rainmeterPath . " [!HideFade GerosMonitor\Taskbar][!HideFade GerosMonitor\Portable]")
    else
        Run(rainmeterPath . " [!ShowFade GerosMonitor\Taskbar][!HideFade GerosMonitor\Portable]")
}

main() {
    if A_Args.Length = 0 {
        Msgbox("This exe can't be ran directly. Refreshing the skin will run it.")
        ExitApp
    }

    rainmeterPath := A_Args[1]
    varsIncPath := A_Args[2]
    windowTitle := "GerosMonitor\Taskbar"
    helpTitle := "GerosMonitor\Help"
    refreshBangs := "[!RefreshGroup Monitors][!Refresh GerosMonitor\Help]"
    writeKeyBang := rainmeterPath . ' [!WriteKeyValue Variables {1} {2} "' . varsIncPath . '"]'
    previousFullscreenState := False

    configString := FileRead(varsIncPath)
    if RegExMatch(configString, "executedOnce=(\d+)", &match)
        executedOnce := match[1]
    if RegExMatch(configString, "hideSkins=(\d+)", &match)
        hideSkins := match[1]
    if RegExMatch(configString, "autoSwitch=(\d+)", &match)
        autoSwitch := match[1]
    if RegExMatch(configString, "autoScale=(\d+)", &match)
        autoScale := match[1]
    if RegExMatch(configString, "scale=(\d+\.?\d*)", &match)
        scale := match[1]

    ; Initial skin settings setup for the scaling and taskbar height
    if not executedOnce {
        Sleep(500)  ; Mayrun too quickly before files are fully placed

        ; Set taskbarHeight to 48 for Windows 11 specifically
        ; Everything else is 40, no idea how best to future proof for Windows 12
        version := StrSplit(A_OSVersion, ".")
        if version[1] == 10 and version[3] >= 22000
            taskbarHeight := 48
        else
            taskbarHeight := 40

        scaleResult := getScale(scale, windowTitle)

        setHeight := Format(writeKeyBang, "taskbarHeight", taskbarHeight)
        setScale := Format(writeKeyBang, "scale", scaleResult)
        setExecuted := Format(writeKeyBang, "executedOnce", "1")
        helpBang := "[!ActivateConfig GerosMonitor\Help]"
        Run(setScale . setHeight . setExecuted . helpBang . refreshBangs)
        ExitApp
    }
    ; If the user is not using the auto features, don't keep running this file
    if not (autoSwitch or autoScale)
        ExitApp
    ; Don't mess with the skin while in the introduction
    while WinExist(helpTitle)
        Sleep(150)

    Loop {
        if not WinExist(windowTitle)
            ExitApp

        if autoScale {
            scaleResult := getScale(scale, windowTitle)

            if scaleResult != scale {
                scale := scaleResult
                setScale := Format(writeKeyBang, "scale", scale)
                Run(setScale . refreshBangs)
            }
        }

        if autoSwitch {
            isFullscreenResult := isFullscreen(previousFullscreenState, windowTitle)

            if isFullscreenResult and (not previousFullscreenState or A_Index = 1)
                isFullscreenAction(rainmeterPath, hideSkins)
            else if not isFullscreenResult and (previousFullscreenState or A_Index = 1)
                isNotFullscreenAction(rainmeterPath, hideSkins)

            previousFullscreenState := isFullscreenResult
        }

        Sleep(150)
    }
}

main()
