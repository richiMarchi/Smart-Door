package com.smart_door.consegna04.smartdoor;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import static java.lang.Thread.sleep;

public class HelloActivity extends Activity {

    private EditText user;
    private EditText password;
    private Button login;
    private Button end;
    public static Handler toSessionHandler;
    public static Handler toMainHandler;
    public static Handler pirDetection;
    public static Handler enableLog;
    public static Handler toHandler;
    private android.support.v7.app.AlertDialog pirDet;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_hello);

        toSessionHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                pirDet.dismiss();
                Intent toSession = new Intent(HelloActivity.this, SessionActivity.class);
                toSession.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
                startActivity(toSession);
            }
        };

        toMainHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                android.support.v7.app.AlertDialog toDialog = new android.support.v7.app.AlertDialog.Builder(HelloActivity.this).create();
                toDialog.setTitle("Timeout");
                toDialog.setMessage("Login timeout triggered");
                toDialog.setButton(DialogInterface.BUTTON_NEGATIVE,"CLOSE", new DialogInterface.OnClickListener(){
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        Intent intent = new Intent(HelloActivity.this, MainActivity.class);
                        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        intent.putExtra("Exit me", true);
                        startActivity(intent);
                        finish();
                    }
                });
                toDialog.setButton(DialogInterface.BUTTON_POSITIVE, "RETRY", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        Intent intent = new Intent(HelloActivity.this, MainActivity.class);
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

        pirDetection = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                pirDet = new android.support.v7.app.AlertDialog.Builder(HelloActivity.this).create();
                pirDet.setTitle("PIR Detection");
                pirDet.setMessage("Login Successful!\nWaiting for PIR Detection to access session window...");
                pirDet.setCanceledOnTouchOutside(false);
                pirDet.setCancelable(false);
                pirDet.show();
            }
        };

        toHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                pirDet.dismiss();
                android.support.v7.app.AlertDialog toDialog = new android.support.v7.app.AlertDialog.Builder(HelloActivity.this).create();
                toDialog.setTitle("Timeout");
                toDialog.setMessage("You did not show in front of PIR Sensor. Retry or close app.");
                toDialog.setButton(DialogInterface.BUTTON_NEGATIVE,"CLOSE", new DialogInterface.OnClickListener(){
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        Intent intent = new Intent(HelloActivity.this, MainActivity.class);
                        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        intent.putExtra("Exit me", true);
                        startActivity(intent);
                        finish();
                    }
                });
                toDialog.setButton(DialogInterface.BUTTON_POSITIVE, "RETRY", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        Intent intent = new Intent(HelloActivity.this, MainActivity.class);
                        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        startActivity(intent);
                        finish();
                        ConnectionManager.getInstance().sendMessage("hm\n");
                    }
                });
                toDialog.show();
            }
        };

        enableLog = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                login.setEnabled(true);
            }
        };

        user = findViewById(R.id.user);
        password = findViewById(R.id.pw);
        login = findViewById(R.id.login);
        end = findViewById(R.id.end);
        login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!user.getText().toString().equals("") && !password.getText().toString().equals("")) {
                    login.setEnabled(false);
                    ConnectionManager.getInstance().sendMessage("user:" + user.getText().toString() + "\n");
                    System.out.println("user:" + user.getText().toString() + "\n");
                    try {
                        sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    ConnectionManager.getInstance().sendMessage("pass:" + password.getText().toString() + "\n");
                    System.out.println("pass:" + password.getText().toString() + "\n");
                    try {
                        sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    ConnectionManager.getInstance().sendMessage("r\n");
                } else {
                    Toast.makeText(HelloActivity.this,"Fill in all credentials", Toast.LENGTH_LONG).show();
                }
            }
        });
        end.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ConnectionManager.getInstance().sendMessage("qm\n");
                Intent intent = new Intent(HelloActivity.this, MainActivity.class);
                intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                intent.putExtra("Exit me", true);
                startActivity(intent);
                finish();
            }
        });
    }

    @Override
    public void onBackPressed() {
        android.support.v7.app.AlertDialog alertDialog = new android.support.v7.app.AlertDialog.Builder(HelloActivity.this).create();
        alertDialog.setTitle("Quit?");
        alertDialog.setMessage("Do you really want to quit?");
        alertDialog.setButton(android.support.v7.app.AlertDialog.BUTTON_NEGATIVE, "NO",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) { }
                });
        alertDialog.setButton(android.support.v7.app.AlertDialog.BUTTON_POSITIVE, "YES",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        ConnectionManager.getInstance().sendMessage("qm\n");
                        Intent intent = new Intent(HelloActivity.this, MainActivity.class);
                        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        intent.putExtra("Exit me", true);
                        startActivity(intent);
                        finish();
                    }
                });
        alertDialog.show();
    }
}
