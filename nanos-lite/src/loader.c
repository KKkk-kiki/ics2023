#include <proc.h>
#include <elf.h>
#include <klib.h>
#include <fs.h>
#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
    int fd = fs_open(filename, 0, 0);
    if(fd < 0){
      panic("should not reach here");
    }
    Elf_Ehdr ehdr;
    fs_read(fd, &ehdr, sizeof(Elf_Ehdr));
    // 检查ELF魔数，确保这是一个有效的ELF文件
    assert(*(uint32_t *)ehdr.e_ident == 0x464c457f);
    //检测ELF文件的ISA类型

    // 读取程序头表
    Elf_Phdr phdr[ehdr.e_phnum];
    fs_lseek(fd, ehdr.e_phoff, SEEK_SET);
    fs_read(fd, phdr, sizeof(Elf_Phdr)*ehdr.e_phnum);
    

    // 遍历程序头表，加载各个段到内存中
    for (int i = 0; i < ehdr.e_phnum; i++) {
      if (phdr[i].p_type == PT_LOAD) {
        // 写入段在内存中的地址
        ramdisk_read((void*)phdr[i].p_vaddr, phdr[i].p_offset, phdr[i].p_memsz);
        // 如果内存大小大于文件大小，将剩余部分清零
        if (phdr[i].p_memsz > phdr[i].p_filesz) {
          memset((void*)(phdr[i].p_vaddr+phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
        }
      }
    }

  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, "/bin/file-test");
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

