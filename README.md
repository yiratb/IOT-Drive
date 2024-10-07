Step 1: Load the NBD Kernel Module

Open a terminal and execute the following command to load the NBD kernel module:

```bash
sudo modprobe nbd
```

Step 2: Configure NBD Sectors

In the same terminal, run the following loop to set the maximum sectors per request for each NBD device:

```bash
for i in {0..15}; do
    sudo bash -c "echo 4 > /sys/block/nbd$i/queue/max_sectors_kb"
done
```

Step 3: Run Minion and Master

Open two separate terminals to run the minion and master processes:

    Terminal 1: Run the Minion:
    
    ```bash
    ./minion_test1.out
    ```

    Terminal 2: Run the Master:

    ```bash
        sudo ./final_project_test.out /dev/nbd0
    ```

Step 4: Prepare the NBD Device

In another terminal, format the NBD device and mount it:

    Format the NBD device:

    ```bash
        sudo mkfs.ext2 /dev/nbd0
    ```

    Mount the NBD device:

    ```bash
        sudo mount /dev/nbd0 /mnt
    ```

Step 5: Access the Mounted Filesystem

You can now access the mounted filesystem at /mnt. Use standard file operations to read and write data.
