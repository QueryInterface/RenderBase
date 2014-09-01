Library.NewObject{
    name        = "Cube",
    elementName = "Cube",
}

Library.NewObject{
    name        = "Cilinder",
    elementName = "Cilinder",
}

Library.NewObject{
    name        = "Wedge",
    elementName = "Wedge",
}

Constructor.Place{name = "Cilinder", position = {16, 0, 16},}
Constructor.Place{name = "Cube",     position = {16, 1, 16},}
Constructor.Place{name = "Cilinder", position = {16, 2, 16},}

Constructor.Place{name = "Wedge", position = {17, 0, 15}, orientation = Directions.pX}
Constructor.Place{name = "Wedge", position = {17, 0, 16}, orientation = Directions.pX}
Constructor.Place{name = "Wedge", position = {17, 0, 17}, orientation = Directions.pX}

Constructor.Place{name = "Wedge", position = {15, 0, 15}, orientation = Directions.nX}
Constructor.Place{name = "Wedge", position = {15, 0, 16}, orientation = Directions.nX}
Constructor.Place{name = "Wedge", position = {15, 0, 17}, orientation = Directions.nX}

Constructor.Place{name = "Wedge", position = {16, 0, 15}, orientation = Directions.nZ}
Constructor.Place{name = "Wedge", position = {16, 0, 17}, orientation = Directions.pZ}