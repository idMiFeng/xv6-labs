struct buf {
  int valid;   // has data been read from disk? 指示缓冲区中的数据是否有效，即是否已从磁盘上读取
  int disk;    // does disk "own" buf?  指示缓冲区当前是否由磁盘所拥有
  uint dev;                              //与缓冲区相关联的设备的标识符
  uint blockno;                        // 与缓冲区相关联的块号
  struct sleeplock lock; //一个用于同步的睡眠锁，以确保对缓冲区的独占访问。
  uint refcnt;           //表示对该缓冲区的引用次数
  struct buf *prev; // LRU cache list        前一个缓冲区的指针
  struct buf *next; //指向LRU缓存列表中后一个缓冲区的指针
  uchar data[BSIZE]; //用于存储缓冲区实际数据的数组
};

