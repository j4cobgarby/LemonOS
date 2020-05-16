#include <apic.h>

#include <cpu.h>
#include <logging.h>
#include <paging.h>
#include <smp.h>
#include <idt.h>
#include <acpi.h>

namespace APIC{
    namespace Local {
        uintptr_t base;
        volatile uintptr_t virtualBase;

        void SpuriousInterruptHandler(regs64_t* r){
            Log::Warning("[APIC] Spurious Interrupt");
        }

        uint64_t ReadBase(){
            uint64_t low;
            uint64_t high;
            asm("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1B));

            return (high << 32) | low;
        }

        void WriteBase(uint64_t val){
            uint64_t low = val & 0xFFFFFFFF;
            uint64_t high = val >> 32;
            asm("rdmsr" :: "a"(low), "d"(high), "c"(0x1B));
        }

        void Enable(){
            WriteBase(ReadBase() | (1UL << 11));

            Write(LOCAL_APIC_SIVR, Read(LOCAL_APIC_SIVR) |  0x1FF  /* Enable APIC, Vector 255*/);
        }

        int Initialize(){
            Local::base = ReadBase() & LOCAL_APIC_BASE;
            virtualBase = Memory::GetIOMapping(base);

            Log::Info("[APIC] Local APIC Base %x (%x)", base, virtualBase);

            IDT::RegisterInterruptHandler(0xFF, SpuriousInterruptHandler);

            Enable();

            return 0;
        }

        volatile uint32_t Read(uint32_t off){
            return *((volatile uint32_t*)(virtualBase + off));
        }
        
        void Write(uint32_t off, uint32_t val){
            *((uint32_t*)(virtualBase + off)) = val;
        }

        void SendIPI(uint8_t destination, uint32_t dsh /* Destination Shorthand*/, uint32_t type, uint8_t vector){
            uint32_t high = ((uint32_t)destination) << 24;
            uint32_t low = dsh | type | ICR_VECTOR(vector);

            Write(LOCAL_APIC_ICR_HIGH, high);
            Write(LOCAL_APIC_ICR_LOW, low);
        }
    }

    namespace IO {
        uintptr_t base = 0;
        uintptr_t virtualBase;
        uint32_t* registerSelect;
        uint32_t* ioWindow;

        uint32_t interrupts;
        uint32_t apicID;

        uint32_t Read32(uint32_t reg){
            *registerSelect = reg;
            return *ioWindow;
        }

        void Write32(uint32_t reg, uint32_t data){
            *registerSelect = reg;
            *ioWindow = data;
        }
        
        uint64_t Read64(uint32_t reg){

        }

        void Write64(uint32_t reg, uint64_t data){
            uint32_t low = data & 0xFFFFFFFF;
            uint32_t high = data >> 32;

            Write32(reg, low);
            Write32(reg + 1, high);
        }

        void Redirect(uint8_t irq, uint8_t vector, uint32_t delivery){
            Write64(IO_APIC_RED_TABLE_ENT(irq), delivery | vector);
        }

        int Initialize(){
            if(!base){
                Log::Error("[APIC] Attempted to initialize I/O APIC without setting base");
                return 1;
            }

            virtualBase = Memory::GetIOMapping(base);
            
            registerSelect = (uint32_t*)(virtualBase + IO_APIC_REGSEL);
            ioWindow = (uint32_t*)(virtualBase + IO_APIC_WIN);

            interrupts = Read32(IO_APIC_REGISTER_VER) >> 16;
            apicID = Read32(IO_APIC_REGISTER_ID) >> 24;

            Log::Info("[APIC] I/O APIC Base %x (%x), Available Interrupts: %d, ID: %d ", base, virtualBase, interrupts, apicID);

            for(int i = 0; i < ACPI::isos->get_length(); i++){
                apic_iso_t* iso = ACPI::isos->get_at(i);
				Log::Info("[APIC] Interrupt Source Override, IRQ: %d, GSI: %d", iso->irqSource, iso->gSI);
                Redirect(iso->gSI, iso->irqSource + 0x20, 0);
            }

            return 0;
        }

        void SetBase(uintptr_t newBase){
            base = newBase;
        }

        void MapLegacyIRQ(uint8_t irq){
            Log::Info("[APIC] Mapping Legacy IRQ, IRQ: %d", irq);
            Redirect(irq, irq + 0x20, 0);
        }
    }

    int Initialize(){
        cpuid_info_t cpuid = CPUID();

        if(!(cpuid.features_edx & CPUID_EDX_APIC)) {
            Log::Error("APIC Not Present");
            return 1;
        }

        asm("cli");

        IDT::DisablePIC();

        Local::Initialize();
        IO::Initialize();

        asm("sti");

        return 0;
    }
}

extern "C" void LocalAPICEOI(){
    APIC::Local::Write(LOCAL_APIC_EOI, 0);
}