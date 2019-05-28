package com.smart_door.consegna04.smartdoor;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.AsyncTask;
import android.widget.Toast;

import java.io.IOException;
import java.util.UUID;

public class ConnectionTask extends AsyncTask<Void, Void, Void> {

    private BluetoothSocket btSocket = null;

    public ConnectionTask(BluetoothDevice targetDevice, UUID uuid) {
        try {
            btSocket = targetDevice.createRfcommSocketToServiceRecord(uuid);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    protected Void doInBackground(Void... voids) {
        try {
            btSocket.connect();
        } catch (Exception l) {
            try {
                btSocket.close();
            } catch (IOException f) {
                f.printStackTrace();
            }
            return null;
        }

        ConnectionManager cm = ConnectionManager.getInstance();
        cm.setChannel(btSocket);
        cm.start();
        ConnectionManager.getInstance().sendMessage("hm\n");

        return null;
    }
}
