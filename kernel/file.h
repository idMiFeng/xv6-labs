// 表示正在被进程打开的文件
struct file {
  enum { FD_NONE, FD_PIPE, FD_INODE, FD_DEVICE } type;  //FD_NONE（未定义类型）、FD_PIPE（管道文件）、FD_INODE（inode 文件）、FD_DEVICE（设备文件）。
  int ref; // reference count      用于记录当前文件被打开的次数
  char readable;     // 表示文件是否可读
  char writable;     // 表示文件是否可写
  struct pipe *pipe; // FD_PIPE
  struct inode *ip;  // FD_INODE and FD_DEVICE
  uint off;          // FD_INODE     文件偏移
  short major;       // FD_DEVICE    设备文件的主设备号
};

#define major(dev)  ((dev) >> 16 & 0xFFFF)
#define minor(dev)  ((dev) & 0xFFFF)
#define	mkdev(m,n)  ((uint)((m)<<16| (n)))

// in-memory copy of an inode
struct inode {
  uint dev;           // Device number
  uint inum;          // Inode number
  int ref;            // Reference count
  struct sleeplock lock; // protects everything below here
  int valid;          // inode has been read from disk?

  short type;         // copy of disk inode
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[NDIRECT+1];
};

// map major device number to device functions.
struct devsw {
  int (*read)(int, uint64, int);
  int (*write)(int, uint64, int);
};

extern struct devsw devsw[];

#define CONSOLE 1
