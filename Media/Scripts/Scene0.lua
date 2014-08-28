Library.NewObject{name = "Cube",     elementName = "Cube",}
Library.NewObject{name = "Cilinder", elementName = "Cilinder",}

for i=0,10 do
    for j=0,3 do
        Constructor.Place{name = "Cube", position = {10+i, i, 10+j},}
    end
end

for i=0,9 do
    for j=0,10 do
        Constructor.Place{name = "Cilinder", position = {20, i, 10+j},}
    end
end

for i=0,10 do
    for j=7,10 do
        Constructor.Place{name = "Cube", position = {10+i, i, 10+j},}
    end
end
