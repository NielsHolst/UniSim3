```mermaid
classDiagram
AverageAllMaterialsInLayer --|> LayerAdjusted : outputs
AverageAllMaterialsInLayer *--> AverageOneMaterialInLayer : holds
AverageCover --|> AverageAllMaterialsInLayer : implements
AverageScreen --|> AverageAllMaterialsInLayer : implements
AverageOneMaterialInLayer --|> LayerAdjusted : outputs
AverageOneMaterialInLayer --|> Layer : inputs
AverageCover --> Face : reads
AverageScreen --> Face : reads
Cover --|> Layer : inputs
Cover --|> LayerAdjusted : outputs
Screen --|> Layer : inputs
Screen --|> LayerAdjusted : outputs
Face ..> Cover : refers to
Face ..> Screen : refers to
Screen --> ActuatorScreen : reads\nstate
Cover --> Shading : reads\nreflectivity
Shading *--> ShadingAgent : holds
Faces *--> Face : holds
materials_covers *--> Cover : holds
materials_screens *--> Screen : holds

class Layer {
+swReflectivityTop
+swReflectivityBottom
+lwReflectivityTop
+lwReflectivityBottom
+swTransmissivityTop
+swTransmissivityBottom
+lwTransmissivityTop
+lwTransmissivityBottom
+swAbsorptivityTop
+swAbsorptivityBottom
+lwAbsorptivityTop
+lwAbsorptivityBottom
+Utop
+Ubottom
+heatCapacity
updateInputsFromProduct(QString productPath)
}

class LayerAdjusted {
+swReflectivityTopAdj
+swReflectivityBottomAdj
+lwReflectivityTopAdj
+lwReflectivityBottomAdj
+swTransmissivityTopAdj
+swTransmissivityBottomAdj
+lwTransmissivityTopAdj
+lwTransmissivityBottomAdj
+swAbsorptivityTopAdj
+swAbsorptivityBottomAdj
+lwAbsorptivityTopAdj
+lwAbsorptivityBottomAdj
+UtopAdj
+UbottomAdj
+heatCapacityAdj
+coverPerGroundArea
}

class AverageOneMaterialInLayer {
+materialName
+myAreas
+myWeights
+allAreas
+allWeights
}

class Cover {
+transmissivityReduction
+swShading
+lwShading
}

class Screen {
+state
+Uinsulation
+UinsulationEffectivity
+energySaving
+UinsulationAdj
}

class Face {
+cover
+screens
+area
+weight
+screenMaterials
}

class AverageCover {
+groundArea
}

class AverageScreen {
+effectiveArea
}
class ActuatorScreen {
+lagPeriod
+desiredState
+timeStepSecs
+state
}
class Shading {
+swReflectivities
+lwReflectivities
+states
+swReflectivity
+lwReflectivity
}
class ShadingAgent {
+swReflectivity
+lwReflectivity
+state
}
```

### Description

Objects of `Cover` and `Screen` has  inputs defined in the `Layer` mix-in class. The boxscript holds `Cover` and `Screen` objects in the `shelter/materials/covers` and `shelter/materials/screens`, respectively. Both covers and screens maintain their `LayerAdjusted` outputs according to the state of shading agents (for covers) and screen actuators (for screens). Screens also maintain their `UinsulationAdj` output.

The `shelter/faces` object holds one `Face` object representing each of the six faces of the greenhouse. In the reset step, the faces makes active those cover and screen products that are used and makes inactive those that are not. This is done to save computing power.

The four layers (cover, screen1, screen2, screen3) may each consist of a mixture of materials. Each layer holds a collection of materials used in that layer. The materials in these collections are each represented by an `AverageOneMaterialInLayer` object, which finds the average weighted by area of that material. The `sw..Adj` and `lw...Adj` are in addition weighted by their face `weight`. The `AverageAllMaterialsLayer` then calculates the total weighted sums of all `...Adj` outputs by summing the values held by its `AverageOneMaterialInLayer` objects.



They use the `updateInputsFromProduct` function to fetch the input values from the proper product.

