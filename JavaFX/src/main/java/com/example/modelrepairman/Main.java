package com.example.modelrepairman;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Screen;
import javafx.stage.Stage;

import java.io.IOException;

/**
 * Приложение для 3 части курсавой работы по "Аналитические модели АСОИУ"
 * Модель службы ремонта и обслуживания ЛВС
 * @author Ilya
 * @version 1.0
 * @since 26.10.2023
 */
public class Main extends Application {
    @Override
    public void start(Stage stage) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(Main.class.getResource("main-view.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setScene(scene);
        stage.setTitle("Модель службы ремонта и обслуживания оборудования ЛВС");
        stage.show();
    }

    public static void main(String[] args) {
        launch();
    }
}