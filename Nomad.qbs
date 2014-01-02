import "Eks/EksBuild" as Eks;

Eks.SubModule {
  name: "Nomad"
  toRoot: "./"

  qbsSearchPaths: base.concat( [ "." ] )

  references: [
    "Eks/EksAll.qbs",
    "Shift/ShiftAll.qbs",
  ]
}
