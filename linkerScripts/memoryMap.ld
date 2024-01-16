MEMORY
{
    sharedram(rwx)      : ORIGIN = 0x20000000, LENGTH = 256
    ram (rwx)           : ORIGIN = 0x20000100, LENGTH = 96K - LENGTH(sharedram)
    bootldr (rw)        : ORIGIN = 0x8000000, LENGTH = 50K
    appPrimary (rx)     : ORIGIN = 0x800C800, LENGTH = 200K
    appSecondary (rx)   : ORIGIN = 0x8040000, LENGTH = 200K
    commondata(rwx)     : ORIGIN = 0x8072a60, LENGTH = 54K
}

SECTIONS
{
    .shared_memory (NOLOAD) : {
        KEEP(*(.shared_memory))
    } >sharedram
}

SECTIONS
{
    .common_data (NOLOAD) : 
    {
        KEEP(*(.common_data))
    } > commondata
}

__sharedram_start__ = ORIGIN(sharedram);
__sharedram_size__ = LENGTH(sharedram);
__bootloader_start__ = ORIGIN(bootldr);
__bootloader_size__ = LENGTH(bootldr);
__app1_start__ = ORIGIN(appPrimary);
__app1_size__ = LENGTH(appPrimary);
__app2_start__ = ORIGIN(appSecondary);
__app2_size__ = LENGTH(appSecondary);
__commondata_start__ = ORIGIN(commondata);