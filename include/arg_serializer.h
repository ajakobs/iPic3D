/*
 * This file contains C-style serializers and deserializers (that rely on C++ classes) for argv arrays
 * @Author: D. Alvarez 
 */

/*
 * This function takes argc and argv, and returns a serialized version of 
 * argv to be reconstructed later, and the size of the resulting array. 
 * The serialized array is a bytecopiable array that contains the following
 * sequence:
 * argc,strlen(argv[0]),argv[0],strlen(argv[1]),argv[1],....
 *
 * Note that if any of the argv values ends or finishes with "," it will
 * cause problems. This probably can be solved more elegantly using C++ classes,
 * but since this is are simple char arrays we can keep it simple. Also my C++-fu
 * is weak, and there is a significant number of things that could have been done
 * better.
 */
char * arg_serializer(int argc, const char **argv);

/*
 * This function takes a serialized version of argv and returns argc and argv.
 *
 * Note that if any of the argv values in the serialized input ends or finishes
 * with "," it will cause problems.
 *
 * This probably can be solved more elegantly using C++ classes,
 * but since this is are simple char arrays we can keep it simple. Also my C++-fu
 * is weak, and there is a significant number of things that could have been done
 * better.
 */
void arg_deserializer(const char *arg_ser, int *argc, char ***argv);


