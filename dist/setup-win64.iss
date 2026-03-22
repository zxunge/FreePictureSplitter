; SPDX-License-Identifier: GPL-3.0-only
; SPDX-FileCopyrightText: 2024-2026 zxunge

#define MyAppName "FreePictureSplitter"
#define MyAppVersion "4.1.0"
#define MyAppPublisher "zxunge"
#define MyAppURL "https://github.com/zxunge/FreePictureSplitter"
#define MyAppExeName "FreePictureSplitter.exe"

[Setup]
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
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
DisableProgramGroupPage=yes
LicenseFile={#MyAppPath}\LICENSE
PrivilegesRequiredOverridesAllowed=commandline
OutputBaseFilename=mysetup
SolidCompression=yes
WizardStyle=modern windows11

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Code]
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
  InstallDir: String;
begin
  InstallDir := ExpandConstant('{app}');

  case CurUninstallStep of
    usUninstall:
      begin
        if DirExists(InstallDir) then
          if MsgBox('Do you want to delete application data?', mbConfirmation, MB_YESNO or MB_DEFBUTTON2) = IDYES then
          begin
            DelTree(InstallDir, True, True, True);
            RemoveDir(ExtractFileDir(InstallDir));
          end;
      end;
  end;
end;

[Files]
Source: "{#MyAppPath}\{#MyAppExeName}"; DestDir: "{app}"
Source: "{#MyAppPath}\*.dll"; DestDir: "{app}"; Flags: skipifsourcedoesntexist
Source: "{#MyAppPath}\LICENSE"; DestDir: "{app}"; Flags: ignoreversion
#ifdef MINGW
  Source: "{#MyAppPath}\symsrv.yes"; DestDir: "{app}"
#endif
#ifndef MINGW
  Source: "{#MyAppPath}\vc_redist.x64.exe"; DestDir: "{tmp}"
#endif
Source: "{#MyAppPath}\plugins\*"; DestDir: "{app}\plugins"; Flags: recursesubdirs createallsubdirs skipifsourcedoesntexist
Source: "{#MyAppPath}\skins\*"; DestDir: "{app}\skins"; Flags: recursesubdirs createallsubdirs skipifsourcedoesntexist
Source: "{#MyAppPath}\translations\*"; DestDir: "{app}\translations"; Flags: recursesubdirs createallsubdirs skipifsourcedoesntexist
Source: "{#MyAppPath}\qt.conf"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
#ifndef MINGW
  Filename: "{tmp}\vc_redist.x64.exe"; Parameters: "/quiet /norestart"; Flags: skipifdoesntexist; StatusMsg: "Installing Microsoft Visual C++ Runtime..."
#endif
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent


