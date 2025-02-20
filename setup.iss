; Example Inno Setup Script
#define MyAppName "GPShield Client App"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "GPShield Technologies"
#define MyAppExeName "GPShieldClientApp.exe"
#define MyAppIcon "E:\GPShield\app.ico"

[Setup]
AppId={{FA04BB14-6EF1-4859-AE56-5F495D3AF025}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
DisableProgramGroupPage=yes
PrivilegesRequired=admin
OutputDir=E:\
OutputBaseFilename=GPShield
SetupIconFile={#MyAppIcon}
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "E:\GPShield\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
; Start Menu Shortcut with Icon
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\app.ico"; IconIndex: 0

; Desktop Shortcut with Icon
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon; IconFilename: "{app}\app.ico"; IconIndex: 0

; Startup Shortcut
Name: "{commonstartup}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\app.ico"; IconIndex: 0

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    if FileExists('C:\Windows\Temp\config.ini') then
      DeleteFile('C:\Windows\Temp\config.ini');
  end;
end;
