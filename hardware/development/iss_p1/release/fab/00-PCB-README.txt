"Thotcon 0x5 Badge Project"

Program Manager:   Mr. Rudy Ristich <rudy@workshop88.com>
Engineer:          Mr. Paul Reich   <reichp@att.net>
                                    [630-924-7289]

PCB fabrication information for the "Thotcon 5 Badge Project".

NOTES:

1. This design is a hybrid SMT/ through hole design. "Component side"
   means that surface of the PC board containing surface mount components.
   "Solder side" means that side of the PCB containing through hole
   components.

3. Print and etch - double-side .062 FR4. Finish copper thickness 1 mil.
   Not for lead-free assembly. Exposed copper to be plated or passivated
   suitable for surface mount assembly.

4. Dimensions in inches, default tolerances unless otherwise specified:

    3 decimals: +/- .005; 2 decimals: +/- .015; 1/64" on fractions.

5. Solder mask and silkscreen color schemes as follow. Note **
   These are minimum required quantities. Round UP to nearest
   full panel.

   SOLDER MASK COLOR  COLOR #    SILK COLOR  COLOR #   TOTAL QUANTITY **
       Flat black       n/a*        WHITE     n/a*       608 min.
       Gloss blue       n/a         WHITE     n/a         40 min.
       Gloss green      n/a         WHITE     n/a        128 min.
       Gloss red        n/a         BLACK     n/a         40 min.
       Gloss "gold"     n/a         BLACK     n/a          8 min.
                                                        ____________
                               GRAND TOTAL               824 min. 

 * Not available, not known
** Round up to panel quantity.
   
6. Drill and route per drawing.  Panelization at vendor discretion,
   but must yield minimum quantities per (5).

7. Dimensions per drawing. Artwork governs positions of undimensioned
   holes.

8. Fabrication drawing is provided as a "Gerber" overlay. Refer to
   files "Dwgs_User.gbr" & "Edge_Cuts.gbr"

FILE MANIFEST:

00-PCB-README.txt   ---       This file.

thotcon_0x5-Dwgs_User.gbr     Fab drawing (gerber format)
thotcon_0x5-Edge_Cuts.gbr     Board outline (gerber)

thotcon_0x5-B_copper.gbl      Solder side copper (gerber) 
thotcon_0x5-F_copper.gtl      Comp. side copper (gerber)

thotcon_0x5-B_Mask.gbs        Solder side solder mask (gerber)
thotcon_0x5-F_Mask.gts        Comp. side solder mask (gerber)

thotcon_0x5-B_SilkS.gbo       Solder side silkscreen (gerber)
thotcon_0x5-F_SilkS.gto       Comp. side silkscreen (gerber)

thotcon_0x5-drl.rpt           Drill size summary report
thotcon_0x5.drl               Excellon drill file, PTH
thotcon_0x5-drl_map.pho       Plated holes (gerber)

thotcon_0x5-NPTH.drl          NON-PLATED Excellon drill file
thotcon_0x5-NPTH-drl_map.pho  Non-plated holes (gerber)

----   FOR REFERENCE ONLY

thotcon_0x5-B_Paste.gbp       Solder side paste
thotcon_0x5-F_Paste.gtp       Comp. side paste.

thotcon_perspective_bottom.jpg  3D rendering [solder side]
thotcon_user_perspective.jpg    3D rendering [comp. side]
