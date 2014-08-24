import "../Eks/EksBuild" as Eks;

Eks.Application {
  name: "NomadEditorApp"
  toRoot: "../"

  cpp.includePaths: base.concat([ "." ])

  files: [ "**/*.h", "**/*.cpp", "**/*.ui", "**/*.qrc", "**/*.png" ]

  Depends { name: "Qt.gui"}
  Depends { name: "Qt.widgets" }

  Depends { name: "Eks3D" }
  Depends { name: "EksGui" }
  Depends { name: "NomadCore" }
  Depends { name: "NomadEditor" }
  Depends { name: "ShiftCore" }
  Depends { name: "ShiftQt" }
}
