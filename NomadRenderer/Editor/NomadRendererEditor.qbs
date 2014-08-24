import qbs	
import "../../Eks/EksBuild" as Eks;

Eks.Library {
  name: "NomadRendererEditor"
  toRoot: "../"

  Depends { name: "ShiftCore" }
  Depends { name: "ShiftQt" }
  Depends { name: "NomadCore" }
  Depends { name: "NomadRenderer" }
  Depends { name: "NomadEditor" }

  Depends { name: "Qt.core" }

  Export {
    Depends { name: "cpp" }

    cpp.includePaths: [ "./include" ]
  }
}
