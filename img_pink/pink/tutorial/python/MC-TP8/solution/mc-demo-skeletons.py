# Michel Couprie, 2011
# CeCILL free-software license

# illustration of filtered Euclidean skeletons

from pink import imview
from pink import manipulate as manipulate
from pink import cpp as pink

global IMAGE
IMAGE = pink.readimage("../images/fish1.pgm")

global LANG
LANG = "en"

def message(mes_en, mes_fr):
    if LANG=="fr":
        print mes_fr
    else:
        print mes_en

message("\n\nDemonstration of skeletonization operators", "\n\nDemonstration des operateurs de squelettisation")
message("Check the 'Superimpose' button in the window", "Activer l'option 'Superimpose' dans l'interface")
message("Click 'Exit' to close the window and continue", "Cliquer sur 'Exit' pour passer au suivant")
print("              ----------------")

message("Discrete lambda-medial axis (DLMA)", "Lambda axe median discret (DLMA)")
def try_DLMA(la): 
    dlma = pink.lambdamedialaxis(IMAGE)
    res = pink.seuil(dlma, la, 0, 255)
    return pink.float2byte(res)

manipulate(try_DLMA, 1, 100, IMAGE)

message("Homotopic skeleton constrained by DLMA", "Squelette homotopique contraint par le DLMA")
def try_DLMA_topo(la): 
    dlma = pink.lambdamedialaxis(IMAGE)
    constr = pink.seuil(dlma, la, 0, 255)
    constr = pink.float2byte(constr)
    res = pink.skeleton(IMAGE, dlma, 8, constr)
    return res

manipulate(try_DLMA_topo, 1, 100, IMAGE)

# LuM end of file
