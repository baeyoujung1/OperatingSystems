#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "defs.h"
#include "x86.h"
#include "elf.h"

int
exec2(char *path, char **argv, int stacksize)
{   
    char *s, *last;
    int i, off;
    uint argc, sz, sp, ustack[3+MAXARG+1];
    struct elfhdr elf;
    struct inode *ip;
    struct proghdr ph;
    pde_t *pgdir, *oldpgdir;
    struct proc *curproc = myproc();

    begin_op();

    if((ip = namei(path)) == 0){
        end_op();
        cprintf("exec: fail\n");
        return -1;
    }
    ilock(ip);
    pgdir = 0;

    // Check ELF header
    if(readi(ip, (char*)&elf, 0, sizeof(elf)) != sizeof(elf))
        goto bad;
    if(elf.magic != ELF_MAGIC)
        goto bad;

    if((pgdir = setupkvm()) == 0)
        goto bad;

    // Load program into memory.
    sz = 0;
    for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
        if(readi(ip, (char*)&ph, off, sizeof(ph)) != sizeof(ph))
        goto bad;
        if(ph.type != ELF_PROG_LOAD)
        continue;
        if(ph.memsz < ph.filesz)
        goto bad;
        if(ph.vaddr + ph.memsz < ph.vaddr)
        goto bad;
        if((sz = allocuvm(pgdir, sz, ph.vaddr + ph.memsz)) == 0)
        goto bad;
        if(ph.vaddr % PGSIZE != 0)
        goto bad;
        if(loaduvm(pgdir, (char*)ph.vaddr, ip, ph.off, ph.filesz) < 0)
        goto bad;
    }
    iunlockput(ip);
    end_op();
    ip = 0;

    // Allocate two pages at the next page boundary.
    // Make the first inaccessible.  Use the second as the user stack.
    
    // 스택용 페이지
    sz = PGROUNDUP(sz);
    for (i = 0; i < stacksize; i++) {
        if((sz = allocuvm(pgdir, sz, sz + PGSIZE)) == 0)
        goto bad;
    }

    // 가드용 페이지
    clearpteu(pgdir, (char*)(sz));
    sp = sz;


    // Push argument strings, prepare rest of stack in ustack.
    for(argc = 0; argv[argc]; argc++) {
        if(argc >= MAXARG)
        goto bad;
        sp = (sp - (strlen(argv[argc]) + 1)) & ~3;
        if(copyout(pgdir, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
        goto bad;
        ustack[3+argc] = sp;
    }
    ustack[3+argc] = 0;

    ustack[0] = 0xffffffff;  // fake return PC
    ustack[1] = argc;
    ustack[2] = sp - (argc+1)*4;  // argv pointer

    sp -= (3+argc+1) * 4;
    if(copyout(pgdir, sp, ustack, (3+argc+1)*4) < 0)
        goto bad;

    // Save program name for debugging.
    for(last=s=path; *s; s++)
        if(*s == '/')
        last = s+1;
    safestrcpy(curproc->name, last, sizeof(curproc->name));

    // Commit to the user image.
    oldpgdir = curproc->pgdir;
    curproc->pgdir = pgdir;
    curproc->sz = sz;
    curproc->tf->eip = elf.entry;  // main
    curproc->tf->esp = sp;
    
    // 스택용 페이지 사이즈 할당
    curproc->stacksize = stacksize;

    switchuvm(curproc);
    freevm(oldpgdir);
    return 0;

    bad:
    if(pgdir)
        freevm(pgdir);
    if(ip){
        iunlockput(ip);
        end_op();
    }
    return -1;
}

int
sys_exec2(void){
	char *path, *argv[MAXARG];
	int stacksize;
	uint argv2;
	
    if(argstr(0, &path) < 0 || argint(1, (int*)&argv2) < 0 || argint(2,&stacksize) < 0)
		return -1;

    memset(argv, 0,sizeof(argv));

    int argc = 0;
    while (argc < MAXARG - 1) {
        if (fetchint((uint)(argv2 + 4 * argc), (int *)&argv[argc]) < 0)
        break;
        argc++;
    }

	if( stacksize < 1 || 100 < stacksize)
		return -1;
	
	return exec2(path, argv, stacksize);
}