#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <string.h>

//#define SPI_DEVICE "/dev/spidev0.2"  // SPI 设备文件路径
char SPI_DEVICE[20]={0};




void dshot_test(int fd){
    printf("dshott test start!\n");
    spi_write(fd,0x00,100);
    spi_write(fd,0x60,75);

    for(int i=0; i<10;++i){
        spi_write(fd, 0x1 ,0x1033 + i);
        usleep(1000);
    }
}
int main(int argc,char *argv[])
{
    int fd;
    uint8_t tx[] = { 0x01, 0x02, 0x03, 0x04 };  // 发送的数据
    uint8_t rx[sizeof(tx)];  // 接收的数据
    int SPI_MODE = atoi(argv[1]);//=SPI_MODE_0;
    strcpy(SPI_DEVICE,argv[2]);

    fd = open(SPI_DEVICE, O_RDWR);  // 打开 SPI 设备
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 设置 SPI 模式和时钟频率
    if (ioctl(fd, SPI_IOC_WR_MODE, &SPI_MODE) < 0) {
        perror("SPI_IOC_WR_MODE");
        exit(EXIT_FAILURE);
    }

    spi_write(fd,0x00,10000);

    uint16_t values[]={0x02,0x03,0x04,0x05};
    //spi_write_datas(fd,0x02,values,sizeof(values)/sizeof(uint8_t));

    for(int i=0;i<8;++i){
        //spi_write(fd,i+1,5000);
        printf("addr %#lx, data:%#lx\n",i,spi_read(fd,i));
    }

    dshot_test(fd);

    close(fd);  // 关闭 SPI 设备

    return 0;
}
