import "../Eks/EksBuild" as Eks;

Eks.Application {
  name: "NomadEditor"
  toRoot: "../"

  files: [ "**/*.h", "**/*.cpp" ]

  Depends { name: "Qt.widgets" }

  Depends { name: "Eks3D" }
  Depends { name: "NomadCore" }
  Depends { name: "ShiftCore" }
}
