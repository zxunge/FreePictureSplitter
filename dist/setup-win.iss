#define MyAppName "FreePictureSplitter"
#define MyAppVersion "4.0.0"
#define MyAppPublisher "zxunge"
#define MyAppURL "https://github.com/zxunge/FreePictureSplitter"
#define MyAppExeName "FreePictureSplitter.exe"

; Your built binaries' path here
#define MyAppPath ""

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{C8F7C428-391D-4E8E-9533-925FA40B6A5A}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
; "ArchitecturesAllowed=x64compatible" specifies that Setup cannot run
; on anything but x64 and Windows 11 on Arm.
ArchitecturesAllowed=x64compatible
; "ArchitecturesInstallIn64BitMode=x64compatible" requests that the
; install be done in "64-bit mode" on x64 or Windows 11 on Arm,
; meaning it should use the native 64-bit Program Files directory and
; the 64-bit view of the registry.
ArchitecturesInstallIn64BitMode=x64compatible
DisableProgramGroupPage=yes
LicenseFile={#MyAppPath}\LICENSE
; Uncomment the following line to run in non administrative install mode (install for current user only).
;PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=commandline
OutputBaseFilename=mysetup
SolidCompression=yes
WizardStyle=modern windows11

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#MyAppPath}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\dbgcore.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\dbghelp.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\exchndl.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libb2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libbrotlicommon.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libbrotlidec.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libbz2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libdouble-conversion.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libfreetype-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libglib-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libgraphite2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libharfbuzz-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libiconv-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libicudt78.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libicuin78.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libicuuc78.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libintl-8.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libjasper.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libjpeg-8.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libmd4c.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libmng-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libpcre2-16-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libpcre2-8-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libpng16-16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libsharpyuv-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libturbojpeg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libwebp-7.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libwebpdecoder-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libwebpdemux-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libwebpmux-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\libzstd.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\LICENSE"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\mgwhelp.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\qt.conf"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\Qt6Concurrent.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\Qt6Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\Qt6Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\Qt6Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\Qt6Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\Qt6Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\symsrv.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\symsrv.yes"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\vulkan-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppPath}\plugins\*"; DestDir: "{app}\plugins"; Flags: recursesubdirs createallsubdirs
Source: "{#MyAppPath}\skins\*"; DestDir: "{app}\skins"; Flags: recursesubdirs createallsubdirs
Source: "{#MyAppPath}\translations\*"; DestDir: "{app}\translations"; Flags: recursesubdirs createallsubdirs

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

