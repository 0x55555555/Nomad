import "Eks/EksBuild" as Eks;

Eks.SubModule {
  name: "Nomad"
  toRoot: "./"

  qbsSearchPaths: base.concat( [ "C:/Users/Jorj/SpaceGamez/Nomad" ] )

  references: [
    "Eks/EksAll.qbs",
    "Shift/ShiftAll.qbs",
  ]
}
