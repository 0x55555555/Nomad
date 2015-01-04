import qbs	
import "../Eks/EksBuild" as Eks;

Eks.Library {
  name: "NomadRenderer"
  toRoot: "../"

  Depends { name: "Eks3D" }
  Depends { name: "ShiftCore" }
  Depends { name: "NomadCore" }
  Depends { name: "RenderCore" }

  Export {
    Depends { name: "cpp" }
    Depends { name: "Eks3D" }
    Depends { name: "RenderCore" }

    cpp.includePaths: [ "./include" ]
  }
}
