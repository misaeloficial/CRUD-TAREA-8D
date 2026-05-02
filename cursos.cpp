#include "cursos.h"
#include "conexion.h"
#include <iostream>
#include <string>

using namespace std;

// =============================================
// Funcion: menuCursos()
// Muestra el submenu del modulo Cursos.
// =============================================
void menuCursos()
{
  int opcion;

  do
  {
    cout << "\n===== MODULO CURSOS =====" << endl;
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
      listarCursos();
      break;
    case 2:
      insertarCurso();
      break;
    case 3:
      actualizarCurso();
      break;
    case 4:
      eliminarCurso();
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
// Funcion: listarCursos()
// Muestra todos los cursos registrados.
// =============================================
void listarCursos()
{
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  const char *sql = "SELECT id, nombre, codigo FROM cursos ORDER BY id";

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql, SQL_NTS);

  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
  {
    cout << "[ERROR] No se pudo ejecutar la consulta." << endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return;
  }

  SQLINTEGER id;
  SQLCHAR nombre[100], codigo[20];
  SQLLEN len;

  cout << "\n--- LISTA DE CURSOS ---" << endl;
  cout << "----------------------------------------" << endl;

  bool hayDatos = false;

  while (SQLFetch(hStmt) == SQL_SUCCESS)
  {
    hayDatos = true;
    SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, &len);
    SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, 100, &len);
    SQLGetData(hStmt, 3, SQL_C_CHAR, codigo, 20, &len);

    cout << "ID: " << id
         << " | Nombre: " << nombre
         << " | Codigo: " << codigo << endl;
  }

  if (!hayDatos)
  {
    cout << "No hay cursos registrados." << endl;
  }

  cout << "----------------------------------------" << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: insertarCurso()
// Pide nombre y codigo del curso y lo guarda.
// =============================================
void insertarCurso()
{
  string nombre, codigo;
  cin.ignore();

  cout << "\n--- GRABAR CURSO ---" << endl;
  cout << "Nombre del curso: ";
  getline(cin, nombre);
  cout << "Codigo del curso: ";
  getline(cin, codigo);

  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "INSERT INTO cursos (nombre, codigo) VALUES ('" + nombre + "', '" + codigo + "')";

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Curso grabado exitosamente." << endl;
  else
    cout << "[ERROR] No se pudo grabar el curso." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: actualizarCurso()
// Modifica el nombre y codigo de un curso.
// =============================================
void actualizarCurso()
{
  int id;
  string nombre, codigo;

  cout << "\n--- MODIFICAR CURSO ---" << endl;
  cout << "ID del curso a modificar: ";
  cin >> id;
  cin.ignore();

  cout << "Nuevo nombre: ";
  getline(cin, nombre);
  cout << "Nuevo codigo: ";
  getline(cin, codigo);

  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "UPDATE cursos SET "
               "nombre='" +
               nombre + "', "
                        "codigo='" +
               codigo + "' "
                        "WHERE id=" +
               to_string(id);

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Curso actualizado correctamente." << endl;
  else
    cout << "[ERROR] No se pudo actualizar el curso." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: eliminarCurso()
// Elimina un curso. Valida primero si tiene
// alumnos asignados para advertir al usuario.
// =============================================
void eliminarCurso()
{
  int id;

  cout << "\n--- ELIMINAR CURSO ---" << endl;
  cout << "ID del curso a eliminar: ";
  cin >> id;

  // Verificar si el curso tiene alumnos asignados
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sqlVerificar = "SELECT COUNT(*) FROM alumnos_cursos WHERE curso_id=" + to_string(id);
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
    cout << "[AVISO] Este curso tiene " << cantidad << " alumno(s) asignado(s)." << endl;
    cout << "Si lo elimina, se quitaran las asignaciones tambien." << endl;
  }

  char confirmar;
  cout << "Esta seguro que desea eliminar el curso con ID " << id << "? (s/n): ";
  cin >> confirmar;

  if (confirmar != 's' && confirmar != 'S')
  {
    cout << "Operacion cancelada." << endl;
    return;
  }

  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "DELETE FROM cursos WHERE id=" + to_string(id);
  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Curso eliminado correctamente." << endl;
  else
    cout << "[ERROR] No se pudo eliminar el curso." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}