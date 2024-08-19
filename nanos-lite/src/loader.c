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
    fs_read(fd, &ehdr, sizeof(ehdr));
    // 检查ELF魔数，确保这是一个有效的ELF文件
    assert(*(uint32_t *)ehdr.e_ident == 0x464c457f);
    //检测ELF文件的ISA类型

    // 读取程序头表
    Elf_Phdr phdr;

    // 遍历程序头表，加载各个段到内存中
    for (int i = 0; i < ehdr.e_phnum; i++) {
      fs_lseek(fd, ehdr.e_phoff + i*ehdr.e_phentsize, SEEK_SET);
      fs_read(fd, &phdr, ehdr.e_phentsize);
      if (phdr.p_type == PT_LOAD) {
        // 写入段在内存中的地址
          // char *buf_malloc = (char*)malloc(phdr.p_memsz);
          // memcpy((void*)phdr.p_vaddr,phdr.p_memsz)
        fs_lseek(fd, phdr.p_offset, SEEK_SET);
        fs_read(fd, (void*)phdr.p_vaddr, phdr.p_memsz);  
        // 如果内存大小大于文件大小，将剩余部分清零
        if (phdr.p_memsz > phdr.p_filesz) {
          memset((void*)(phdr.p_vaddr+phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
        }
      }
    }

  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

