import qbs	
import "../Eks/EksBuild" as Eks;

Eks.Library {
  name: "NomadRenderer"
  toRoot: "../"

  Depends { name: "Eks3D" }
  Depends { name: "ShiftCore" }
  Depends { name: "NomadCore" }

  Export {
    Depends { name: "cpp" }
    Depends { name: "Eks3D" }

    cpp.includePaths: [ "./include" ]
  }
}
