package com.smart_door.consegna04.smartdoor;

import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;

import java.nio.BufferUnderflowException;

import static java.lang.Thread.sleep;

public class SessionActivity extends Activity {

    private TextView temperature;
    private SeekBar seekBar;
    private TextView brightness;
    private Button logout;
    public static Handler closeHandler;
    public static Handler toHandler;
    public static Handler setTemp;
    public static Handler setBright;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_session);

        closeHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                android.support.v7.app.AlertDialog toDialog = new android.support.v7.app.AlertDialog.Builder(SessionActivity.this).create();
                toDialog.setTitle("Session closed");
                toDialog.setMessage("Session closed by peer. Closing app...");
                toDialog.setButton(DialogInterface.BUTTON_NEUTRAL,"OK", new DialogInterface.OnClickListener(){
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        Intent intent = new Intent(SessionActivity.this, MainActivity.class);
                        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        intent.putExtra("Exit me", true);
                        startActivity(intent);
                        finish();
                    }
                });
                toDialog.setCancelable(false);
                toDialog.setCanceledOnTouchOutside(false);
                toDialog.show();
            }
        };

        toHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                android.support.v7.app.AlertDialog toDialog = new android.support.v7.app.AlertDialog.Builder(SessionActivity.this).create();
                toDialog.setTitle("Timeout");
                toDialog.setMessage("Session timeout triggered");
                toDialog.setButton(DialogInterface.BUTTON_NEGATIVE,"CLOSE", new DialogInterface.OnClickListener(){
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        Intent intent = new Intent(SessionActivity.this, MainActivity.class);
                        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        intent.putExtra("Exit me", true);
                        startActivity(intent);
                        finish();
                    }
                });
                toDialog.setButton(DialogInterface.BUTTON_POSITIVE, "RETRY", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        Intent intent = new Intent(SessionActivity.this, MainActivity.class);
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

        setTemp = new Handler() {
            @Override
            public void handleMessage(Message temp) {
                temperature.setText(temp.obj + " °");
            }
        };

        setBright = new Handler() {
            @Override
            public void handleMessage(Message bright) {
                brightness.setText(bright.obj + " %");
                seekBar.setProgress(Integer.parseInt((String)bright.obj));
            }
        };

        temperature = findViewById(R.id.temp);
        seekBar = findViewById(R.id.seek);
        brightness = findViewById(R.id.brightness);
        logout = findViewById(R.id.logout);
        logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ConnectionManager.getInstance().sendMessage("end\n");
                Intent intent = new Intent(SessionActivity.this, MainActivity.class);
                intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                intent.putExtra("Exit me", true);
                startActivity(intent);
                finish();
            }
        });
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                brightness.setText(seekBar.getProgress() + " %");
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                ConnectionManager.getInstance().sendMessage("lval:" + seekBar.getProgress() + "\n");
            }
        });
        ConnectionManager.getInstance().sendMessage("sc\n");
    }

    @Override
    public void onBackPressed() {
        android.support.v7.app.AlertDialog alertDialog = new android.support.v7.app.AlertDialog.Builder(SessionActivity.this).create();
        alertDialog.setTitle("Quit?");
        alertDialog.setMessage("Do you really want to quit?");
        alertDialog.setButton(android.support.v7.app.AlertDialog.BUTTON_NEGATIVE, "NO",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) { }
                });
        alertDialog.setButton(android.support.v7.app.AlertDialog.BUTTON_POSITIVE, "YES",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        ConnectionManager.getInstance().sendMessage("end\n");
                        Intent intent = new Intent(SessionActivity.this, MainActivity.class);
                        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        intent.putExtra("Exit me", true);
                        startActivity(intent);
                        finish();
                    }
                });
        alertDialog.show();
    }
}
