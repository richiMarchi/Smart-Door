package com.smart_door.consegna04.smartdoor;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AlertDialog;
import android.os.Bundle;
import android.widget.Toast;

import java.util.Set;
import java.util.UUID;

public class MainActivity extends Activity {

    public static Context context;
    private BluetoothAdapter btAdapter;
    private static final int REQUEST_ENABLE_BT = 1;
    private static final String TARGET_DEVICE = "bt13";
    private static final String COMM_ID = "00001101-0000-1000-8000-00805F9B34FB";

    private Set<BluetoothDevice> nbDevices = null;
    private BluetoothDevice targetDevice = null;
    public static Handler toHelloHandler;
    public static Handler toHandler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if( getIntent().getBooleanExtra("Exit me", false)){
            finish();
            System.exit(0);
        }

        toHelloHandler = new Handler(){
            @Override
            public void handleMessage(Message msg) {
                Intent toHello = new Intent(MainActivity.this, HelloActivity.class);
                toHello.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
                startActivity(toHello);
            }
        };

        toHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                android.support.v7.app.AlertDialog toDialog = new android.support.v7.app.AlertDialog.Builder(MainActivity.this).create();
                toDialog.setTitle("Timeout");
                toDialog.setMessage("Waiting timeout triggered");
                toDialog.setButton(DialogInterface.BUTTON_NEGATIVE,"CLOSE", new DialogInterface.OnClickListener(){
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        Intent intent = new Intent(MainActivity.this, MainActivity.class);
                        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        intent.putExtra("Exit me", true);
                        startActivity(intent);
                        finish();
                    }
                });
                toDialog.setButton(DialogInterface.BUTTON_POSITIVE, "RETRY", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        Intent intent = new Intent(MainActivity.this, MainActivity.class);
                        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        startActivity(intent);
                        finish();
                        ConnectionManager.getInstance().sendMessage("hm\n");
                    }
                });
                toDialog.setCancelable(false);
                toDialog.setCanceledOnTouchOutside(false);
                toDialog.show();
            }
        };

        context = getApplicationContext();
        btAdapter = BluetoothAdapter.getDefaultAdapter();

        if (btAdapter == null) {
            AlertDialog alertDialog = new AlertDialog.Builder(MainActivity.this).create();
            alertDialog.setTitle("App cannot work");
            alertDialog.setMessage("This application cannot be used because the device does not support bluetooth.");
            alertDialog.setButton(AlertDialog.BUTTON_NEUTRAL, "Close",
                    new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            dialog.dismiss();
                            finish();
                        }
                    });
            alertDialog.show();
        } else {
            if (!btAdapter.isEnabled()) {
                Intent i = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(i, REQUEST_ENABLE_BT);
            } else {
                nbDevices = btAdapter.getBondedDevices();

                if (nbDevices.size() > 0) {
                    for (BluetoothDevice btDev : nbDevices) {
                        if (btDev.getName().equals(TARGET_DEVICE)) {
                            targetDevice = btDev;
                            UUID uuid = UUID.fromString(COMM_ID);
                            new ConnectionTask(targetDevice, uuid).execute();
                            Toast.makeText(MainActivity.this, "Wait until connection is established, be patient...", Toast.LENGTH_LONG).show();
                        }
                    }
                }
            }
        }
    }

    @Override
    public void onActivityResult(int reqID , int res , Intent data ){
        if( reqID == REQUEST_ENABLE_BT && res == Activity.RESULT_OK ){

            nbDevices = btAdapter.getBondedDevices();

            if (nbDevices.size() > 0) {
                for (BluetoothDevice btDev : nbDevices) {
                    if (btDev.getName().equals(TARGET_DEVICE)) {
                        targetDevice = btDev;
                        UUID uuid = UUID.fromString(COMM_ID);
                        new ConnectionTask(targetDevice, uuid).execute();
                        Toast.makeText(MainActivity.this, "Wait until connection is established, be patient...", Toast.LENGTH_LONG).show();
                    }
                }
            }
        }
        if( reqID == REQUEST_ENABLE_BT && res == Activity.RESULT_CANCELED ){
            Toast.makeText(MainActivity.this,"App cannot work without bluetooth activation.",Toast.LENGTH_LONG).show();
            finish();
        }
    }

    @Override
    public void onBackPressed() {
        android.support.v7.app.AlertDialog alertDialog = new android.support.v7.app.AlertDialog.Builder(MainActivity.this).create();
        alertDialog.setTitle("Quit?");
        alertDialog.setMessage("Do you really want to quit?");
        alertDialog.setButton(android.support.v7.app.AlertDialog.BUTTON_NEGATIVE, "NO", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) { }
        });
        alertDialog.setButton(android.support.v7.app.AlertDialog.BUTTON_POSITIVE, "YES",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        ConnectionManager.getInstance().sendMessage("qm\n");
                        Intent intent = new Intent(MainActivity.this, MainActivity.class);
                        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        intent.putExtra("Exit me", true);
                        startActivity(intent);
                        finish();
                    }
                });
        alertDialog.show();
    }
}
