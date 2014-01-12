import "../Eks/EksBuild" as Eks;

Eks.Test {
  name: "NomadCoreTest"
  toRoot: "../../"

  Depends { name: "NomadCore" }
  Depends { name: "ShiftCore" }
}
