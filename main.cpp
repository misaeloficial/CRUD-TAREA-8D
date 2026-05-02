// =============================================
// Proyecto: Sistema Universidad - CRUD en C++
// Base de datos: PostgreSQL via ODBC
// Curso: Programacion I
// =============================================

// Incluimos todos los modulos del proyecto
#include "conexion.h"
#include "alumnos.h"
#include "cursos.h"
#include "secciones.h"
#include "alumnos_cursos.h"
#include "alumnos_seccion.h"

// =============================================
// Funcion: mostrarMenuPrincipal()
// Es el menu central del sistema.
// Desde aqui se accede a todos los modulos.
// =============================================
void mostrarMenuPrincipal()
{
  int opcion;

  do
  {
    cout << "\n========================================" << endl;
    cout << "      SISTEMA UNIVERSIDAD               " << endl;
    cout << "========================================" << endl;
    cout << "  1. Alumnos" << endl;
    cout << "  2. Cursos" << endl;
    cout << "  3. Secciones" << endl;
    cout << "  4. Alumnos - Cursos" << endl;
    cout << "  5. Alumnos - Seccion" << endl;
    cout << "  6. Salir" << endl;
    cout << "========================================" << endl;
    cout << "Seleccione una opcion: ";
    cin >> opcion;

    switch (opcion)
    {
    case 1:
      menuAlumnos();
      break;
    case 2:
      menuCursos();
      break;
    case 3:
      menuSecciones();
      break;
    case 4:
      menuAlumnosCursos();
      break;
    case 5:
      menuAlumnosSeccion();
      break;
    case 6:
      cout << "\nSaliendo del sistema..." << endl;
      break;
    default:
      cout << "Opcion invalida. Intente de nuevo." << endl;
    }

  } while (opcion != 6);
}

// =============================================
// Funcion principal: main()
// Punto de entrada del programa.
// 1. Intenta conectarse a la base de datos.
// 2. Si conecta, muestra el menu principal.
// 3. Al salir, cierra la conexion.
// =============================================
int main()
{
  // Intentar conectarse a la base de datos
  if (!conectar())
  {
    cout << "No se pudo iniciar el sistema. Verifique la conexion." << endl;
    return 1; // termina el programa con error
  }

  // Mostrar el menu principal
  mostrarMenuPrincipal();

  // Cerrar la conexion antes de salir
  cerrarConexion();

  return 0; // termina el programa correctamente
}