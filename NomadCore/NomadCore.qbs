import qbs	
import "../Eks/EksBuild" as Eks;

Eks.Library {
  name: "NomadCore"
  toRoot: "../"

  Depends { name: "Qt.core" }
  Depends { name: "ShiftCore" }
  Depends { name: "ShiftQt" }

  Export {
    Depends { name: "cpp" }

    cpp.includePaths: [ "./include" ]
  }
}
