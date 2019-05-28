package com.smart_door.consegna04.smartdoor;

import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.v7.app.AlertDialog;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.regex.Pattern;

public class ConnectionManager extends Thread {

    private BluetoothSocket btSocket;
    private InputStream btInStream;
    private OutputStream btOutStream;

    private boolean stop;

    private static ConnectionManager instance = null;

    private ConnectionManager() {
        stop = true;
    }

    public static ConnectionManager getInstance(){
        if(instance == null) {
            instance = new ConnectionManager();
        }
        return instance;
    }

    public void setChannel(BluetoothSocket socket) {
        btSocket = socket;

        try {
            btInStream = socket.getInputStream();
            btOutStream = socket.getOutputStream();
        } catch (IOException e) {
            e.printStackTrace();
        }
        stop = false;
    }

    public void run() {
        byte[] buffer = new byte[1024];
        int nBytes = 0;
        String s = "";

        while (!stop) {
            try {
                nBytes = btInStream.read(buffer);
                s += new String(Arrays.copyOf(buffer, nBytes));
                if (buffer[nBytes - 1] == 10) {
                    String l = s.substring(0,s.length() - 2);
                    manageMessage(l);
                    s = "";
                }
            } catch (IOException e) {
                stop = true;
            }
        }
    }

    public boolean write(byte[] bytes) {
        if(btOutStream == null) {
            return false;
        }
        try {
            btOutStream.write(bytes);
        } catch (IOException e) {
            return false;
        }
        return true;
    }

    public void cancel() {
        try {
            btSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void sendMessage(String msg) {
        write(msg.getBytes());
    }

    private void manageMessage(String buffer) {
        switch (buffer) {
            case "h":
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        MainActivity.toHelloHandler.handleMessage(new Message());
                    }
                });
                break;

            case "aOK":
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        HelloActivity.pirDetection.handleMessage(new Message());
                    }
                });
                break;

            case "aFAIL":
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.context,"Access denied. Check your credentials.",Toast.LENGTH_LONG).show();
                        HelloActivity.enableLog.handleMessage(new Message());
                    }
                });
                break;

            case "gwERR":
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.context, "Gateway error, retry now", Toast.LENGTH_LONG).show();
                        HelloActivity.enableLog.handleMessage(new Message());
                    }
                });
                break;

            case "RSTon":
                //waiting for presence
                //MainActivity.toHandler.handleMessage(new Message());
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        MainActivity.toHandler.handleMessage(new Message());
                    }
                });
                break;

            case "RSTal":
                //waiting for login
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        HelloActivity.toMainHandler.handleMessage(new Message());
                    }
                });
                break;

            case "RSTls":
                //waiting for pir detection
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        HelloActivity.toHandler.handleMessage(new Message());
                    }
                });
                break;

            case "sSTART":
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        HelloActivity.toSessionHandler.handleMessage(new Message());
                    }
                });
                break;

            case "q":
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        SessionActivity.closeHandler.handleMessage(new Message());
                    }
                });
                break;

            case "sTOUT":
                new Handler(Looper.getMainLooper()).post(new Runnable() {
                    @Override
                    public void run() {
                        SessionActivity.toHandler.handleMessage(new Message());
                    }
                });
                break;

            default:
                final String[] sep = buffer.split(Pattern.quote("|"));
                if (sep[0].equals("3")) {
                    new Handler(Looper.getMainLooper()).post(new Runnable() {
                        @Override
                        public void run() {
                            Message msg = new Message();
                            msg.obj = sep[1];
                            SessionActivity.setBright.handleMessage(msg);
                        }
                    });
                } else if (sep[0].equals("2")) {
                    new Handler(Looper.getMainLooper()).post(new Runnable() {
                        @Override
                        public void run() {
                            Message msg = new Message();
                            msg.obj = sep[1];
                            SessionActivity.setTemp.handleMessage(msg);
                        }
                    });
                } else {
                    //do nothing to avoid app from crashing. It's something not in the message list.
                }
                break;
        }
    }
}
