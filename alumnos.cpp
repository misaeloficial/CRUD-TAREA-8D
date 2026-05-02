#include "alumnos.h"
#include "conexion.h"
#include <iostream>
#include <string>

using namespace std;

// =============================================
// Funcion: menuAlumnos()
// Muestra el submenu del modulo Alumnos y
// llama a la funcion correspondiente segun
// la opcion que elija el usuario.
// =============================================
void menuAlumnos()
{
  int opcion;

  do
  {
    cout << "\n===== MODULO ALUMNOS =====" << endl;
    cout << "1. Listar" << endl;
    cout << "2. Grabar" << endl;
    cout << "3. Modificar" << endl;
    cout << "4. Eliminar" << endl;
    cout << "5. Regresar" << endl;
    cout << "Seleccione: ";
    cin >> opcion;

    switch (opcion)
    {
    case 1:
      listarAlumnos();
      break;
    case 2:
      insertarAlumno();
      break;
    case 3:
      actualizarAlumno();
      break;
    case 4:
      eliminarAlumno();
      break;
    case 5:
      cout << "Regresando al menu principal..." << endl;
      break;
    default:
      cout << "Opcion invalida. Intente de nuevo." << endl;
    }

  } while (opcion != 5);
}

// =============================================
// Funcion: listarAlumnos()
// Muestra todos los alumnos con sus cursos
// y seccion asignada usando JOIN.
// Si no tiene cursos muestra "Sin cursos".
// Si no tiene seccion muestra "Sin seccion".
// =============================================
void listarAlumnos()
{
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  // Consulta con JOIN para obtener datos de todas las tablas
  const char *sql =
      "SELECT a.id, a.nombres, a.apellidos, a.carnet, "
      "COALESCE(c.nombre, 'Sin cursos') AS curso, "
      "COALESCE(s.nombre, 'Sin seccion') AS seccion "
      "FROM alumnos a "
      "LEFT JOIN alumnos_cursos ac ON a.id = ac.alumno_id "
      "LEFT JOIN cursos c ON ac.curso_id = c.id "
      "LEFT JOIN alumnos_seccion ase ON a.id = ase.alumno_id "
      "LEFT JOIN secciones s ON ase.seccion_id = s.id "
      "ORDER BY a.id";

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql, SQL_NTS);

  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
  {
    cout << "[ERROR] No se pudo ejecutar la consulta." << endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return;
  }

  // Variables para guardar cada columna del resultado
  SQLINTEGER id;
  SQLCHAR nombres[100], apellidos[100], carnet[20];
  SQLCHAR curso[100], seccion[100];
  SQLLEN len;

  cout << "\n--- LISTA DE ALUMNOS ---" << endl;
  cout << "------------------------------------------------------------" << endl;

  bool hayDatos = false;

  // SQLFetch recorre fila por fila los resultados
  while (SQLFetch(hStmt) == SQL_SUCCESS)
  {
    hayDatos = true;
    SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, &len);
    SQLGetData(hStmt, 2, SQL_C_CHAR, nombres, 100, &len);
    SQLGetData(hStmt, 3, SQL_C_CHAR, apellidos, 100, &len);
    SQLGetData(hStmt, 4, SQL_C_CHAR, carnet, 20, &len);
    SQLGetData(hStmt, 5, SQL_C_CHAR, curso, 100, &len);
    SQLGetData(hStmt, 6, SQL_C_CHAR, seccion, 100, &len);

    cout << "ID: " << id
         << " | Nombre: " << nombres << " " << apellidos
         << " | Carnet: " << carnet
         << " | Curso: " << curso
         << " | Seccion: " << seccion << endl;
  }

  if (!hayDatos)
  {
    cout << "No hay alumnos registrados." << endl;
  }

  cout << "------------------------------------------------------------" << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: insertarAlumno()
// Pide los datos al usuario y los inserta
// en la tabla alumnos.
// =============================================
void insertarAlumno()
{
  string nombres, apellidos, carnet;

  // cin.ignore() limpia el buffer para que getline funcione bien
  cin.ignore();

  cout << "\n--- GRABAR ALUMNO ---" << endl;
  cout << "Nombres   : ";
  getline(cin, nombres);
  cout << "Apellidos : ";
  getline(cin, apellidos);
  cout << "Carnet    : ";
  getline(cin, carnet);

  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "INSERT INTO alumnos (nombres, apellidos, carnet) VALUES ('" + nombres + "', '" + apellidos + "', '" + carnet + "')";

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Alumno grabado exitosamente." << endl;
  else
    cout << "[ERROR] No se pudo grabar el alumno." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: actualizarAlumno()
// Pide el ID del alumno a modificar y los
// nuevos datos, luego los actualiza en la BD.
// =============================================
void actualizarAlumno()
{
  int id;
  string nombres, apellidos, carnet;

  cout << "\n--- MODIFICAR ALUMNO ---" << endl;
  cout << "ID del alumno a modificar: ";
  cin >> id;
  cin.ignore();

  cout << "Nuevos nombres   : ";
  getline(cin, nombres);
  cout << "Nuevos apellidos : ";
  getline(cin, apellidos);
  cout << "Nuevo carnet     : ";
  getline(cin, carnet);

  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "UPDATE alumnos SET "
               "nombres='" +
               nombres + "', "
                         "apellidos='" +
               apellidos + "', "
                           "carnet='" +
               carnet + "' "
                        "WHERE id=" +
               to_string(id);

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Alumno actualizado correctamente." << endl;
  else
    cout << "[ERROR] No se pudo actualizar el alumno." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: eliminarAlumno()
// Pide el ID del alumno y lo elimina.
// Tambien elimina sus asignaciones por el
// ON DELETE CASCADE definido en el SQL.
// =============================================
void eliminarAlumno()
{
  int id;

  cout << "\n--- ELIMINAR ALUMNO ---" << endl;
  cout << "ID del alumno a eliminar: ";
  cin >> id;

  // Confirmacion antes de eliminar
  char confirmar;
  cout << "Esta seguro que desea eliminar al alumno con ID " << id << "? (s/n): ";
  cin >> confirmar;

  if (confirmar != 's' && confirmar != 'S')
  {
    cout << "Operacion cancelada." << endl;
    return;
  }

  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "DELETE FROM alumnos WHERE id=" + to_string(id);

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Alumno eliminado correctamente." << endl;
  else
    cout << "[ERROR] No se pudo eliminar el alumno." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}