#include "secciones.h"
#include "conexion.h"
#include <iostream>
#include <string>

using namespace std;

// =============================================
// Funcion: menuSecciones()
// Muestra el submenu del modulo Secciones.
// =============================================
void menuSecciones()
{
  int opcion;

  do
  {
    cout << "\n===== MODULO SECCIONES =====" << endl;
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
      listarSecciones();
      break;
    case 2:
      insertarSeccion();
      break;
    case 3:
      actualizarSeccion();
      break;
    case 4:
      eliminarSeccion();
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
// Funcion: listarSecciones()
// Muestra todas las secciones registradas.
// =============================================
void listarSecciones()
{
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  const char *sql = "SELECT id, nombre, jornada FROM secciones ORDER BY id";

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql, SQL_NTS);

  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
  {
    cout << "[ERROR] No se pudo ejecutar la consulta." << endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return;
  }

  SQLINTEGER id;
  SQLCHAR nombre[50], jornada[50];
  SQLLEN len;

  cout << "\n--- LISTA DE SECCIONES ---" << endl;
  cout << "----------------------------------------" << endl;

  bool hayDatos = false;

  while (SQLFetch(hStmt) == SQL_SUCCESS)
  {
    hayDatos = true;
    SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, &len);
    SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, 50, &len);
    SQLGetData(hStmt, 3, SQL_C_CHAR, jornada, 50, &len);

    cout << "ID: " << id
         << " | Nombre: " << nombre
         << " | Jornada: " << jornada << endl;
  }

  if (!hayDatos)
  {
    cout << "No hay secciones registradas." << endl;
  }

  cout << "----------------------------------------" << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: insertarSeccion()
// Pide nombre y jornada y guarda la seccion.
// =============================================
void insertarSeccion()
{
  string nombre, jornada;
  cin.ignore();

  cout << "\n--- GRABAR SECCION ---" << endl;
  cout << "Nombre de la seccion : ";
  getline(cin, nombre);
  cout << "Jornada (Ej: Matutina, Vespertina, Nocturna): ";
  getline(cin, jornada);

  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "INSERT INTO secciones (nombre, jornada) VALUES ('" + nombre + "', '" + jornada + "')";

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Seccion grabada exitosamente." << endl;
  else
    cout << "[ERROR] No se pudo grabar la seccion." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: actualizarSeccion()
// Modifica nombre y jornada de una seccion.
// =============================================
void actualizarSeccion()
{
  int id;
  string nombre, jornada;

  cout << "\n--- MODIFICAR SECCION ---" << endl;
  cout << "ID de la seccion a modificar: ";
  cin >> id;
  cin.ignore();

  cout << "Nuevo nombre  : ";
  getline(cin, nombre);
  cout << "Nueva jornada : ";
  getline(cin, jornada);

  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "UPDATE secciones SET "
               "nombre='" +
               nombre + "', "
                        "jornada='" +
               jornada + "' "
                         "WHERE id=" +
               to_string(id);

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Seccion actualizada correctamente." << endl;
  else
    cout << "[ERROR] No se pudo actualizar la seccion." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: eliminarSeccion()
// Elimina una seccion. Valida si tiene alumnos
// asignados y pide confirmacion al usuario.
// =============================================
void eliminarSeccion()
{
  int id;

  cout << "\n--- ELIMINAR SECCION ---" << endl;
  cout << "ID de la seccion a eliminar: ";
  cin >> id;

  // Verificar si la seccion tiene alumnos asignados
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sqlVerificar = "SELECT COUNT(*) FROM alumnos_seccion WHERE seccion_id=" + to_string(id);
  SQLExecDirect(hStmt, (SQLCHAR *)sqlVerificar.c_str(), SQL_NTS);

  SQLINTEGER cantidad = 0;
  SQLLEN len;
  if (SQLFetch(hStmt) == SQL_SUCCESS)
  {
    SQLGetData(hStmt, 1, SQL_C_LONG, &cantidad, 0, &len);
  }
  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

  if (cantidad > 0)
  {
    cout << "[AVISO] Esta seccion tiene " << cantidad << " alumno(s) asignado(s)." << endl;
    cout << "Si la elimina, se quitaran las asignaciones tambien." << endl;
  }

  char confirmar;
  cout << "Esta seguro que desea eliminar la seccion con ID " << id << "? (s/n): ";
  cin >> confirmar;

  if (confirmar != 's' && confirmar != 'S')
  {
    cout << "Operacion cancelada." << endl;
    return;
  }

  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "DELETE FROM secciones WHERE id=" + to_string(id);
  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Seccion eliminada correctamente." << endl;
  else
    cout << "[ERROR] No se pudo eliminar la seccion." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}