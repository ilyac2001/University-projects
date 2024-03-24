/**
 * Класс управления для View:
 * - about-programm (о программе, окно открывается по кнопке, опсание работы, условные обозначения и автора);
 * - main (главное окно приложения, ввод данных, отображение расчётов и графиков)
 */
package com.example.modelrepairman;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.XYChart;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyEvent;
import javafx.stage.Modality;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.Objects;
import java.util.function.Consumer;

@FunctionalInterface
interface NumberFunction<T> {
    double apply(T model);
}
public class VIewController {
    @FXML
    private RadioButton radioButtonY;
    @FXML
    private RadioButton radioButtonL;
    @FXML
    private RadioButton radioButtonTp;
    private ToggleGroup toggleGroup;
    @FXML
    public LineChart chart;
    @FXML
    public TableView<Model> table;
    @FXML
    Button btn_result;
    @FXML
    TextField tH0, t0, N, C_var1, C_var2, C_var3, S1, S;
    @FXML
    TextField decimal_point;
    private ObservableList<Model> models = FXCollections.observableArrayList();
    XYChart.Series<Number, Number>[] series;
    @FXML
    private void initialize() {
        this.tH0.setText("1000");
        this.t0.setText("12");
        this.N.setText("100");
        this.C_var1.setText("1");
        this.C_var2.setText("2");
        this.C_var3.setText("3");
        this.S1.setText("3500");
        this.S.setText("1000");

        chart.setTitle("Зависимость убытков, среднего кол-ва компьютеров и среднего времени пребывания ПК в неисправном состоянии от кол-ва ремонтников");

        toggleGroup = new ToggleGroup();
        radioButtonY.setToggleGroup(toggleGroup);
        radioButtonL.setToggleGroup(toggleGroup);
        radioButtonTp.setToggleGroup(toggleGroup);
        radioButtonY.setOnAction(event -> {
            chart.getData().clear();
            chart.getData().add(series[0]);
        });
        radioButtonL.setOnAction(event -> {
            chart.getData().clear();
            chart.getData().add(series[1]);
        });
        radioButtonTp.setOnAction(event -> {
            chart.getData().clear();
            chart.getData().add(series[2]);
        });

    }

    /**
     * Расчёт значений Model, последующий вывод результата в таблицу
     * @param event
     */
    @FXML
    private void GetResult(ActionEvent event) {
        AddVariantModel();
        //вывод таблицы
        table.setItems(models);
        //добавление точек на график
        SetDataForChart();
        //вывод 3 графиков
        chart.getData().clear();
        radioButtonY.setSelected(false);
        radioButtonL.setSelected(false);
        radioButtonTp.setSelected(false);
        for(int i = 0; i < models.size(); i++) {
            chart.getData().add(series[i]);
        }
    }

    /**
     * Открывает окно с информацией о программе
     * @param event
     */
    @FXML
    public void OpenNewStage(ActionEvent event) throws IOException {
        Stage imageStage = new Stage();
        imageStage.initModality(Modality.APPLICATION_MODAL);
        imageStage.setTitle("О программе: условные обозначения и об авторе");

        Image img = new Image(getClass().getResourceAsStream("/images/about-program.jpg"));
        ImageView imgView = new ImageView(img);
        Group root = new Group(imgView);

        imageStage.setScene(new Scene(root));
        imageStage.show();
    }

    /**
     * Создание 3-х вариантов модели для сравнения
     */
    private void AddVariantModel(){
        models.clear();
        models.add(0, new Model(Integer.parseInt(tH0.getText()),
                Integer.parseInt(t0.getText()),
                Integer.parseInt(N.getText()),
                Integer.parseInt(C_var1.getText()),
                Integer.parseInt(S1.getText()),
                Integer.parseInt(S.getText())));
        models.add(1, new Model(Integer.parseInt(tH0.getText()),
                Integer.parseInt(t0.getText()),
                Integer.parseInt(N.getText()),
                Integer.parseInt(C_var2.getText()),
                Integer.parseInt(S1.getText()),
                Integer.parseInt(S.getText())));
        models.add(2, new Model(Integer.parseInt(tH0.getText()),
                Integer.parseInt(t0.getText()),
                Integer.parseInt(N.getText()),
                Integer.parseInt(C_var3.getText()),
                Integer.parseInt(S1.getText()),
                Integer.parseInt(S.getText())));
    }

    /**
     * Установака значений для 3-х графиков
     */
    private void SetDataForChart(){
        //очищение значений координат
        series = new XYChart.Series[models.size()];
        for(int i = 0; i < models.size(); i++) {
            series[i] = new XYChart.Series<>();
        }
        //установка точек графиков
        SetDataForChart("Y", 0, Model::getY);
        SetDataForChart("L", 1, Model::getL);
        SetDataForChart("Tp", 2, Model::getTp);
    }

    /**
     * Создание серии точек, зависимость от числа ремонтников
     * @param nameChart
     * @param indexSeries
     * @param valueExtractor - Getter переменная модели значение которой требуется проследить
     */
    private void SetDataForChart(String nameChart, int indexSeries, NumberFunction<Model> valueExtractor) {
        series[indexSeries].setName(nameChart);
        for(int i = 0; i < this.models.size(); i++) {
            this.series[indexSeries].getData().add(
                    new XYChart.Data<>(this.models.get(i).getC(),
                            valueExtractor.apply(this.models.get(i))));
        }
    }
    @FXML
    public void handleTextChanged(KeyEvent event) {
        if(!decimal_point.getText().isEmpty() && decimal_point.getText().matches("\\d+")) {
            Model.setDecimal_point(Integer.parseInt(decimal_point.getText()));
        } else {
            Model.setDecimal_point(Model.NOT_DECIMAL_POINT);
        }

        table.getItems().forEach(row -> {
            table.getColumns().forEach(col -> {
                col.setVisible(false);
                col.setVisible(true);
            });
        });
    }
}