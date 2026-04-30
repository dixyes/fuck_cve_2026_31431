# fuck_cve_2026_31431

```bash
make KERNELVER=`uname -r`
: for LKM
sudo insmod fuck-cve-2026-31431.ko reg_fake_impl=1
: for builtin
sudo insmod fuck-cve-2026-31431.ko
```
