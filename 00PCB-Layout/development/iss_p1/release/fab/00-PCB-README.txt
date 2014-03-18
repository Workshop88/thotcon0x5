"Thotcon 0x5 Badge Project"

Program Manager:   Mr. Rudy Ristich <rudy@workshop88.com>
Engineer:          Mr. Paul Reich   <reichp@att.net>

This package contains PCB fabrication information for
the "Thotcon 5 Badge Project".

NOTES:

1. Due to the LCD display, this design is hybrid SMT/
through hole.

2. "Component side" means that surface of the PC board
containing surface mount components. "Solder side" means
that side of the PCB containing through hole components.

3. Print and etch - double-side .062 FR4. Finish copper
thickness 1 mil. Not for lead-free assembly. Exposed
copper to be plated or passivated suitable for surface
mount assembly.

4. Solder mask GREEN; Silkscreen ink WHITE.

5. Drill and route per drawing.  Panelization per vendor
requirement. Dimensions per drawing. Locations of 
undimensioned holes governed by artwork.

6. Hole location tolerances where specified:

    3 decimals: +/- .005;
    2 decimals: +/- .010

7. The fabrication drawing is provided as a "Gerber"
overlay. Refer to files "Dwgs_User.gbr" & "Edge_Cuts.gbr"

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

   (REFERENCE ONLY)

thotcon_0x5-B_Paste.gbp       Solder side paste
thotcon_0x5-F_Paste.gtp       Comp. side paste.

thotcon_perspective_bottom.jpg  3D rendering [solder side]
thotcon_user_perspective.jpg    3D rendering [comp. side]
