FILE:  00-ASSEMBLY-README.TXT 

April 11 2014 rev. P1

CHANGES:

    1.  Do not place white tape on "gold" and red boards.
    2.  Do not put white tape on DS1. Delete white tape from BOM.
    3.  (4a) Do not remove manufacturer stickers from display.
    4.  ADD Note A, Red and black switches.

Project: "Thotcon0x5 Badge Project" - An electric name tag.

Customer: Workshop 88, Inc.
Engineer: Paul Reich <reichp@att.net> -- 630-924-7289.
Project Manager: Rudy Ristich <rudy@workshop88.com>

Client: Thotcon 0x5 Organizing Committee.

NOTES:

   A.  Place RED switches at S1 and S2.  Place BLACK switches at
       S3, S4, S5, S6, & S7. Failure to do is a visual defect.

   1.  Please advise ASAP as to the suitability of the enclosed
       files, particularly the centroid file (thotcon_0x5-all.pos),
       should any file conversions be required.
       
   2.  A quantity of parts sufficient to assemble a minimum of 850
       units will be delivered for assembly.
       
       Over-runs, scrap, and leftover material must be returned.
       Please return unpopulated PCBs and panels as well.

   3.  Manufacture each of the following colors as follows:

        SOLDER MASK         SILKSCREEN        Quantity
   
       Gloss "gold"           BLACK           8 minimum
       Gloss red              BLACK          40   "
       Gloss blue             WHITE          40   "
       Gloss green            WHITE         128   "
       Flat black             WHITE         608   "
                                            ____________
                              GRAND TOTAL   824 minimum

   4.  DS1 - Glass display:
           Glass display DS1 must be fixtured such that the under-
           side of the display is spaced .04" - .07" above the PCB
           surface.  Failure to do so is a DEFECT that will leave
           the displays susceptible to mechanical and thermal stress
           and breakage.

   5.  There are a several unused components on each assembly. These
       components are designated "DO NOT PLACE" or "P/O PCB" on the
       bill of material. Holes for "DO NOT PLACE" components must
       remain free of solder.

   6.  Battery holders must be positioned with opening (cutout) as
       shown on the silkscreen.
       
   7.  Build ALL GOLD and RED panels FIRST. Build ALL GREEN and BLUE
       panels next. Build the BLACK panels LAST. These panels are
       the most numerous. Shortfalls here can be accommodated more
       readily.
       
   8.  Finished goods must be packaged in a manner sufficient to
       protect against ESD hazards & breakage of the (glass) display
       unit. Antistat bags or antistat bubble wrap are acceptable.
       Like colors must be packaged or grouped together. 
       
   9.  Customer will install batteries and program units.

  10.  Completed units and other materials, as noted in (2) and (3)
       will be retrieved by the customer (or his agent) at your Elk
       Grove Village location.


********************************************************************

           -----------  FILE MANIFEST  ------------------

File name                                      Description
------------------------------------------------------------------
00-ASSEMBLY-README.1ST                    This file

thotcon_0x5-all.pos                       SMD pick and place file.
thotcon_0x5-F_Paste.gtp                   SMD side paste mask, Gerber
thotcon_0x5-B_SilkS-MIRROR.pdf            Through-hole silk, PDF (mirrored)
thotcon_0x5-Dwgs_User.pdf                 PCB Fab/Assembly dwg, PDF.
thotcon_0x5-F_SilkS.pdf                   SMD-side silkscreen PDF


REFERENCE:

File name                                      Description
----------------------------------------------------------------------
thotcon_0x5_SCHEM-A.pdf                  "A" size schematic, PDF format
thotcon_0x5_SCHEM-B.pdf                  "B" size (11x17) schematic, PDF
thotcon_0x5_BOM_BY_REF.pdf                Bill of material, PDF
thotcon_0x5-drl.rpt                       Drill report, text

thotcon_0x5-F_copper.pdf                  Front copper, PDF
thotcon_0x5-B_copper.pdf                  Back copper, PDF

thotcon_0x5-F_Mask.pdf                    Front mask, PDF
thotcon_0x5-B_Mask.pdf                    Back mask, PDF


04-01-2014 Thotcon0X5 Issue P1 release

EOF