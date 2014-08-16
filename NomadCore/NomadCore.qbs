import qbs	
import "../Eks/EksBuild" as Eks;

Eks.Library {
  name: "NomadCore"
  toRoot: "../"

  Depends { name: "Qt.core" }
  Depends { name: "ShiftCore" }

  Export {
    Depends { name: "cpp" }

    cpp.includePaths: [ "./include" ]
  }
}
