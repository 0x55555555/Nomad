import qbs	
import "../Eks/EksBuild" as Eks;

Eks.Library {
  name: "NomadRenderer"
  toRoot: "../"

  Depends { name: "ShiftCore" }
  Depends { name: "NomadCore" }

  Export {
    Depends { name: "cpp" }

    cpp.includePaths: [ "./include" ]
  }
}
